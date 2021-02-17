#include "memlayout.h"

/**
 * If USER_PAGE_SIZE is defined, PAGE_SIZE is equal to USER_PAGE_SIZE.
 * Otherwise, PAGE_SIZE is 4096.
 */
// #ifdef USER_PAGE_SIZE
// const unsigned int PAGE_SIZE = USER_PAGE_SIZE;
// #else
// const unsigned int PAGE_SIZE = 4096;
// #endif

/**
 * @brief Used by get_mem_layout to test to what extent can we access a page
 * 
 * When a signal is handled, we simply return to where we last called sigsetjmp,
 * and ignore the exception.
 * 
 * @param sig The signal number which handle_signal handled
 */
void handle_signal(int sig)
{
    if (sig == SIGSEGV) {
       (void)signal(SIGSEGV, SIG_IGN);
    } else if (sig == SIGBUS) {
        (void)signal(SIGBUS, SIG_IGN);
    }

    siglongjmp(env, 1);
}

/**
 * @brief Returns how many memory regions there are in the process's address space
 * and lists them in @p regions
 * 
 * The function does so by iterating through every page within the address space,
 * attempting to push what they are allowed to do. If it fails on first try,
 * it's memory you have no access to. If it fails afterwards, it's read-only.
 * If it succeeds all the way through, it's read-writeable.
 * 
 * @param regions list of memory regions @see memregion for memory region definition
 * @param size of list @p regions
 * @return int count of memory regions ( @see memregion ) in the precess's address space
 */
int get_mem_layout(struct memregion *regions, unsigned int size)
{
    const unsigned int REGION_START = 0x0u;
    const unsigned int REGION_END = 0xffffffffu;

    int jump_val;
    int check_val; // where we decide what accessibility is for the current pafe
    unsigned int curr_index = 0u; // current index we're writing on regions
    unsigned int region_from = REGION_START;
    int curr_region_mode = MEM_NO; // current region mode
    bool is_first_loop = true;

    for (unsigned int curr_add = region_from; curr_add < REGION_END; curr_add += PAGE_SIZE)
    {
        /*
         * Hacky solution:
         * 
         * When unsigned int exceeds, it wraps. We stop once we go back to the start.
         * I was supposed to use unsigned long, but I encountered a warning
         * because I was converting unsigned long to char *. I just sticked to my initial
         * solution.
         */
        if (curr_add < PAGE_SIZE)
        {
            if (is_first_loop)
            {
                is_first_loop = false;
            }
            else
            {
                break;
            }
        }

        (void)signal(SIGSEGV, handle_signal);
        (void)signal(SIGBUS, handle_signal);
        jump_val = sigsetjmp(env, 1);
        check_val = MEM_NO;

        // try you're best to abuse your privileges on the current page
        if (jump_val == 0)
        {
            char p1 = ((char *)curr_add)[0];
            jump_val = sigsetjmp(env, 1);
            check_val = MEM_RO;

            if (jump_val == 0)
            {
                ((char *)curr_add)[0] = p1;
                check_val = MEM_RW;
                // can read-write page

            } // else: read-only page

        } // else: cannot read page

        /*
         * If the current page has a different memory accessibility as the current
         * memory region, we start a new memory region. We try to write the memory
         * region (if we still have space in regions) before we start a new one.
         * We increment our index, even if it exceeds the size. The index will also
         * be used for determining the size.
         */
        if (check_val != curr_region_mode)
        {
            if (curr_index < size)
            {
                regions[curr_index].from = (void *)region_from;
                regions[curr_index].to = (void *)curr_add;
                regions[curr_index].mode = curr_region_mode;
                ++curr_index;
            }

            region_from = curr_add;
            curr_region_mode = check_val;
        }
    }

    /*
     * We assume that there's at least one memory region. Our final memory region
     * is assumed to extend to 0xffffffff.
     */
    if (curr_index < size)
    {
        regions[curr_index].from = (void *)region_from;
        regions[curr_index].to = (void *)REGION_END;
        regions[curr_index].mode = curr_region_mode;
    }

    return curr_index + 1;
}

/**
 * @brief Prints a list of memory regions or @see memregion
 * 
 * @param regions list of memory regions or @see memregion modified by @see get_mem_layout
 * @param size size of the list @p regions
 * @param region_count how many regions were counted by @see get_mem_layout
 */
void print_mem_layout(struct memregion *regions, unsigned int size, int region_count)
{
    if (regions == NULL)
    {
        return;
    }

    for (unsigned int i = 0; i < size && i < (unsigned int)region_count; ++i)
    {
        /* 
         * References:
         * (1) Jonathan Leffler. Correct format specifier to print pointer or address? 
         * stackoverflow.com/questions/9053658/correct-format-specifier-to-print-pointer-or-address
         * (2) aschepler. Printing leading zeroes for hexadecimal in C.
         * stackoverflow.com/questions/12062916/printing-leading-zeroes-for-hexadecimal-in-c/12062957
         */
        printf(
            "0x%08" PRIXPTR "-0x%08" PRIXPTR " %s\n", 
            (uintptr_t)regions[i].from, 
            (uintptr_t)regions[i].to, 
            mode_to_string(regions[i].mode)
        );
    }
}

/**
 * @brief Converts @see memregion#mode to const char*
 * 
 * @param mode @see memregion#mode
 * @return const char* 
 */
const char* mode_to_string(unsigned char mode) {
    switch(mode) {
        case MEM_RW:
            return "RW";
        case MEM_RO:
            return "RO";
        case MEM_NO:
            return "NO";
        default:
            return "Unknown";
    }
}
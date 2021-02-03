/* todo: implement */

#include "memlayout.h"

#ifdef USER_PAGE_SIZE
const unsigned int PAGE_SIZE = USER_PAGE_SIZE;
#else
const unsigned int PAGE_SIZE = 4096;
#endif

sigjmp_buf env;
sigjmp_buf ro_env;
sigjmp_buf no_env;

struct memregion *new_memregion(void *from, unsigned char mode)
{
    struct memregion *m = malloc(sizeof *m);
    m->from = from;
    m->to = NULL;
    m->mode = mode;
    return m;
}

void handle_segfault(__attribute__((unused)) int sig)
{
    (void)signal(SIGSEGV, SIG_IGN);
    siglongjmp(no_env, 1);
}

int get_mem_layout(struct memregion *regions, unsigned int size)
{

    /*
    We always know that the memory is NO at start
    */
    // todo: assert regions exists and its size, i.e., the number ofentries, is equal to size
    // todo: change numbers
    // assumption 32 bit

    // assume first address is NO
    int no_jump_val;
    int check_val;
    unsigned int curr_index = 0;
    unsigned int region_from = 0x0;
    int curr_region_mode = MEM_NO;
    int first_loop = 1;
    for (unsigned long curr_add = region_from; curr_add < 0xffffffff; curr_add += PAGE_SIZE)
    {
        if (curr_add < PAGE_SIZE)
        {
            if (first_loop)
            {
                first_loop = 0;
            }
            else
            {
                break;
            }
        }

        (void)signal(SIGSEGV, handle_segfault);
        (void)signal(SIGBUS, handle_segfault);
        no_jump_val = sigsetjmp(no_env, 1);
        check_val = MEM_NO;

        if (no_jump_val == 0)
        {
            char p1 = ((char *)curr_add)[0];
            no_jump_val = sigsetjmp(no_env, 1);
            check_val = MEM_RO;

            if (no_jump_val == 0)
            {
                ((char *)curr_add)[0] = p1;
                check_val = MEM_RW;
            }
        }

        if (check_val != curr_region_mode)
        {
            if (curr_index < size)
            {
                regions[curr_index].from = (void *)region_from;
                regions[curr_index].to = (void *)curr_add;
                regions[curr_index].mode = curr_region_mode;
                ++curr_index;
            }

            // todo: increase size
            //    printf("%d: %x-%x %d\n", curr_index, region_from, curr_add-1, check_val);
            region_from = curr_add;
            curr_region_mode = check_val;
        }
    }

    if (curr_index < size)
    {
        regions[curr_index].from = (void *)region_from;
        regions[curr_index].to = (void *)0xffffffff;
        regions[curr_index].mode = curr_region_mode;
    }

    return curr_index + 1;
}

void print_mem_layout(struct memregion *regions, unsigned int size, int region_count)
{
    if (regions == NULL)
    {
        return;
    }

    // todo: print memlayout
    for (unsigned int i = 0; i < size && i < (unsigned int)region_count; ++i)
    {
        /* 
         * Reference:
         * (1) https://stackoverflow.com/questions/9053658/correct-format-specifier-to-print-pointer-or-address
         * (2) https://stackoverflow.com/questions/12062916/printing-leading-zeroes-for-hexadecimal-in-c/12062957
         */
        printf(
            "0x%08" PRIXPTR "-0x%08" PRIXPTR " %s\n", 
            (uintptr_t)regions[i].from, 
            (uintptr_t)regions[i].to, 
            mode_to_string(regions[i].mode)
        );
    }
}

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
/**
 * @file windowset.c
 * @brief Windowset is a queue and hashtable that keeps track of the windowset size
 * 
 * @remark define DEBUG_PRINT if you want to print details about the queue
 */

#include "windowset.h"

static int page_exponent = 4;
static ull page_size = 16;
static ull window_size = 100;

static struct queue *queue;
static struct hashtable *hashtable;

/**
 * @brief 
 * @brief Initialize window set
 * 
 * Initialize everything required for the window set to work.
 * 
 * @param page_size_ 
 * @param window_size_ 
 */
void initialize_window_set(ull page_size_, ull window_size_) {
    page_size = page_size_;
    window_size = window_size_;

    /* Calculate page exponent: Assuming not page_size > 0*/
    page_exponent = get_power_of_two(page_size);
    
    queue = new_queue();

    // todo: initialize set
    hashtable = new_hashtable();
}

/**
 * @brief Breaks down a mem_ref into individual addresses and puts them into the hashtable
 * 
 * @param mem_ref 
 */
static void windowset_put(struct mem_ref mem_ref) {
    ull address = mem_ref.address;
    for (ull i = 0; i < mem_ref.page_count; ++i) {
        put(hashtable, address + i);
    }
}

/**
 * @brief Breaks down a mem_ref into individual addresses and puts them into
 * 
 * @param mem_ref 
 */
static void windowset_delete(struct mem_ref mem_ref) {
    ull address = mem_ref.address;
    for (ull i = 0; i < mem_ref.page_count; ++i) {
        delete(hashtable, address + i);
    }
}

/**
 * @brief Insert new memory reference into window set
 * 
 * This automatically purges old memory references
 * 
 * @param address 
 * @param page_size 
 */
void window_set_insert(ull address, ull page_size) {
    struct mem_ref mem_ref;

    /* Simplify address and page size */
    ull lower_address = address >> page_exponent;

    /* Reduce by 1 to count own position too */
    ull upper_address = (address + page_size - 1) >> page_exponent; 
    ull page_count = (upper_address - lower_address) + 1;

    mem_ref.address = lower_address;
    mem_ref.page_count = page_count;

    enqueue(queue, mem_ref); // sliding window doubly-linked list
    windowset_put(mem_ref); // put in hashtable

    if (queue->size > window_size) {
        mem_ref = dequeue(queue); // queue
        windowset_delete(mem_ref); // hashtable
    }
}

/**
 * @brief Get the size of the window set
 * 
 * @return ull 
 */
ull get_window_set_size() {
    return hashtable->count;
}

/**
 * @brief Destroy windowset
 */
void destroy_window_set() {
    destroy_queue(queue);
    destroy_hashtable(hashtable);
}

#ifdef DEBUG_PRINT
/**
 * @brief Print details about the queue for debugging purposes
 */
void window_set_debug() {
    queue_debug(queue);
}
#endif /* DEBUG_PRINT */
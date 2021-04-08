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
    page_exponent = -1;
    ull value = page_size;
    while(value != 0) {
        ++page_exponent;
        value = value >> 1;
    }

    queue = new_queue();

    // todo: initialize set
    hashtable = new_hashtable();
}

static void windowset_put(struct mem_ref mem_ref) {
    ull address = mem_ref.address;
    for (ull i = 0; i < mem_ref.page_count; ++i) {
        put(hashtable, address);
    }
}

static void windowset_delete(struct mem_ref mem_ref) {
    ull address = mem_ref.address;
    for (ull i = 0; i < mem_ref.page_count; ++i) {
        delete(hashtable, address);
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
    printf("Processed: %llX, %llu\n", lower_address, page_count);

    mem_ref.address = lower_address;
    mem_ref.page_count = page_count;

    enqueue(queue, mem_ref);

    // /* todo: hashset */
    windowset_put(mem_ref);

    if (queue->size > window_size) {
        mem_ref = dequeue(queue);

        // todo: remove mem_ref from hashset
        windowset_delete(mem_ref);
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
    // todo: clean up

    destroy_queue(queue);
    destroy_hashtable(hashtable);
}


/**
 * @brief todo: delete
 * 
 */
void window_set_debug() {
    printf("Queue size: %llu\n", queue->size);
    queue_debug(queue);
}
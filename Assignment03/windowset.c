#include "windowset.h"

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

    queue = new_queue();

    // todo: initialize set
    hashtable = new_hashtable();
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
    // todo: window_set_insert(ull address, ull page_size)
    struct mem_ref mem_ref;
    mem_ref.address = address;
    mem_ref.page_size = page_size;

    enqueue(queue, mem_ref);

    /* todo: hashset */
    put(hashtable, mem_ref);

    if (queue->size > window_size) {
        mem_ref = dequeue(queue);

        // todo: remove mem_ref from hashset
        delete(hashtable, mem_ref);
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
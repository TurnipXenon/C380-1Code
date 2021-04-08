#include "windowset.h"

static ull page_size = 16;
static ull window_size = 100;
static ull set_size = 0;

static struct queue *queue;

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

    queue = create_queue();

    // todo: initialize set
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

    if (queue->size > window_size) {
        mem_ref = dequeue(queue);

        // todo: remove mem_ref from hashset
    }
}

/**
 * @brief Get the size of the window set
 * 
 * @return ull 
 */
ull get_window_set_size() {
    return set_size;
}

/**
 * @brief Destroy windowset
 */
void destroy_window_set() {
    // todo: clean up

    destroy_queue(queue);
}


/**
 * @brief todo: delete
 * 
 */
void window_set_debug() {
    printf("Queue size: %llu\n", queue->size);
    queue_debug(queue);
}
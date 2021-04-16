#include "queue.h"

/**
 * @brief Create a node object
 * 
 * @remark Default value is 0u
 * 
 * @return struct queue_node* 
 */
struct queue_node *new_node() {
    struct queue_node *node = (struct queue_node*) malloc(sizeof(struct queue_node));
    node->head = NULL;
    node->tail = NULL;
    node->value = new_mem_ref(); // default value
    return node;
}

/**
 * @brief Destroy node
 * 
 * @param node 
 */
void destroy_node(struct queue_node *node) {
    free(node);
}

/**
 * @brief Create a queue object
 * 
 * @return struct queue* 
 */
struct queue *new_queue() {
    struct queue *queue = (struct queue*) malloc(sizeof(struct queue));
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0u;
    return queue;
}

/**
 * @brief Enqueue a node on the head
 * 
 * @param queue 
 * @param value 
 */
void enqueue(struct queue *queue, struct mem_ref value) {
    struct queue_node *node = new_node();
    node->value = value;

    if (queue->head == NULL) {
        /* Case: empty */
        queue->head = node;
        queue->tail = node;
    } else {
        /* Case: non-empty */
        queue->head->head = node;
        node->tail = queue->head;
        queue->head = node;
    }
    
    queue->size++;
}

/**
 * @brief Dequeue a value from the tail node
 * 
 * @param queue 
 * @return ull 
 */
struct mem_ref dequeue(struct queue *queue) {
    struct mem_ref value;

    if (queue->tail != NULL) {
        /* Case: non-empty */
        struct queue_node *node = queue->tail;
        value = node->value;

        if (queue->size == 1) {
            /* Case: only one item */
            queue->head = NULL;
            queue->tail = NULL;
        } else {
            /* Case: more than one item */
            queue->tail = node->head;
            queue->tail->tail = NULL;
        }

        destroy_node(node);
        queue->size--;
    } else {
        /* Case: empty */
        value = new_mem_ref();
        perror("Attempt to dequeue when queue is empty! Returning empty mem_ref.\n");
    }
    
    return value;
}

/**
 * @brief Destroy queue object
 * 
 * @param queue 
 */
void destroy_queue(struct queue *queue) {
    if (queue == NULL) {
        return;
    }

    // destroy tail first
    while(queue->size != 0) {
        dequeue(queue);
    }

    free(queue);
}

#ifdef DEBUG_PRINT
/**
 * @brief Print details about the queue for debugging purposes
 * 
 * @param queue 
 */
void queue_debug(struct queue *queue) {
    printf("Queue: ");

    struct queue_node *node = queue->head;

    while(node != NULL) {
        printf("{%llX, %llu}-> ", node->value.address, node->value.page_count);
        node = node->tail;
    }

    printf("NULL\n");
}
#endif /* DEBUG_PRINT */
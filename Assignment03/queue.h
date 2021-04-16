/**
 * @file queue.h
 * @brief Doubly-linked list used as a queue for checking out which memory reference
 * is already out of the windowset
 * 
 * @remark define DEBUG_PRINT to gain access to queue_debug()
 */

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "valws379_base.h"

/**
 * @brief Doubly-linked node
 */
struct queue_node {
    struct mem_ref value;
    struct queue_node *head;
    struct queue_node *tail;
};

/**
 * @brief Doubly-linked list used as a queue
 */
struct queue {
    ull size;
    struct queue_node *head;
    struct queue_node *tail;
};

/**
 * @brief Create a queue object
 * 
 * @return struct queue* 
 */
struct queue *new_queue();

/**
 * @brief Enqueue a node on the head
 * 
 * @param queue 
 * @param value 
 */
void enqueue(struct queue *queue, struct mem_ref value);

/**
 * @brief Dequeue a value from the tail node
 * 
 * @param queue 
 * @return struct mem_ref 
 */
struct mem_ref dequeue(struct queue *queue);

/**
 * @brief Clean up or destroy queue object
 * 
 * @param queue 
 */
void destroy_queue(struct queue *queue);

#ifdef DEBUG_PRINT
/**
 * @brief Print details about the queue for debugging purposes
 * 
 * @param queue 
 */
void queue_debug(struct queue *queue);
#endif /* DEBUG_PRINT */

#endif /* _QUEUE_H_ */
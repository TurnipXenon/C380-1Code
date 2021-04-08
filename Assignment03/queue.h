#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdlib.h> /* for malloc */
#include <stdio.h> /* for perror */

#include "valws379_base.h"

struct queue_node {
    struct mem_ref value;
    struct queue_node *head;
    struct queue_node *tail;
};

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
struct queue *create_queue();

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

/**
 * @brief todo: delete
 * 
 * @param queue 
 */
void queue_debug(struct queue *queue);

#endif /* _QUEUE_H_ */
/**
 * @file linkedlist.h
 * @brief Singly-linked list for the hashtable
 * 
 * @remark define DEBUG_PRINT to print whenever something is added or removed
 */

#ifndef _LINKED_LIST_
#define _LINKED_LIST_

#include "valws379_base.h"

/**
 * @brief Singly-linked node
 */
struct sll_node {
    struct key_value key_value;
    struct sll_node *next;
};

/**
 * @brief Singly-linked list with weird behaviors for windowset
 */
struct linked_list {
    ull count;
    struct sll_node *head;
};

/**
 * @brief Properly allocate and initialize a linked list
 * 
 * @return struct linked_list* 
 */
struct linked_list *new_linked_list();

/**
 * @brief Remove head and set it to key_value. Return false if there's no element inside
 * the linked list.
 * 
 * @param linked_list 
 * @param key_value 
 * @return true 
 * @return false 
 */
bool sll_pop(struct linked_list *linked_list, struct key_value *key_value);

/**
 * @brief Properly clean up linked list
 * 
 * @param linked_list 
 */
void destroy_linked_list(struct linked_list *linked_list);

/**
 * @brief Increment a value with the matching key, otherwise add a new node 
 * for the key-value pair.
 * 
 * @param linked_list 
 * @param key 
 */
void sll_add(struct linked_list *linked_list, ull key);

/**
 * @brief Decrement a value with the matching key. If the value reaches zero, the node
 * is deleted.
 * 
 * @param linked_list 
 * @param key 
 */
void sll_remove(struct linked_list *linked_list, ull key);

/**
 * @brief Puts a key value pair as it is on the head
 * 
 * @param linked_list 
 * @param key_value 
 */
void sll_put_key_value(struct linked_list *linked_list, struct key_value key_value);

#endif
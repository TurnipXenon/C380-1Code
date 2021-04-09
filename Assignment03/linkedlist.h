#ifndef _LINKED_LIST_
#define _LINKED_LIST_

#include "valws379_base.h"

struct sll_node {
    struct key_value key_value;
    struct sll_node *next;
}

/**
 * @brief Singly-linked list with weird behaviors for windowset
 */
struct linked_list {
    int count;
    struct sll_node *head;
}

struct linked_list *new_linked_list();

void destroy_linked_list(struct linked_list *linked_list);

// struct key_value *find(struct linked_list *linked_list, ull key);

void add(struct linked_list *linked_list, ull key);

void remove(struct linked_list *linked_list, ull key);

#endif
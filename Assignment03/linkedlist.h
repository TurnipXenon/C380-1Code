#ifndef _LINKED_LIST_
#define _LINKED_LIST_

#include "valws379_base.h"

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

struct linked_list *new_linked_list();

bool sll_pop(struct linked_list *linked_list, struct key_value *key_value);

void destroy_linked_list(struct linked_list *linked_list);

// struct key_value *find(struct linked_list *linked_list, ull key);

void sll_add(struct linked_list *linked_list, ull key);

void sll_remove(struct linked_list *linked_list, ull key);

void sll_put_key_value(struct linked_list *linked_list, struct key_value key_value);

#endif
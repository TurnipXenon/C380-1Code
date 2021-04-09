#include "linkedlist.h"

struct linked_list *new_linked_list() {
    struct linked_list *new = (struct linked_list*) malloc(sizeof(struct linked_list));
    new->count = 0;
    new->head = NULL;
    return new;
}

void destroy_linked_list(struct linked_list *linked_list) {
    struct sll_node *node = linked_list->head;
    struct sll_node *old_node;

    while (node != NULL) {
        old_node = node;
        node = node->next;
        free(old_node);
    }

    free(linked_list);
}

static struct sll_node *find(struct linked_list *linked_list, ull key) {
    struct sll_node *node = linked_list->head;

    while(node != NULL) {
        if (node->key_value.key == key) {
            break;
        }
        node = node->next;
    }

    return node;
}

static void insert(struct linked_list *linked_list, ull key) {
    struct sll_node *new_head = (struct sll_node*) malloc(sizeof(struct sll_node));
    new_head->key_value.key = key;
    new_head->key_value.value = 1u;
    
    if (linked_list->head != NULL) {
        new_head->next = linked_list->head;
    } else {
        new_head->next = NULL;
    }

    linked_list->head = new_head;
}

void add(struct linked_list *linked_list, ull key) {
    struct sll_node *node = find(linked_list, key);

    if (node != NULL) {
        node->key_value.value++;
    } else {
        linked_list->count++;
        insert(linked_list, key);
    }
}

void remove(struct linked_list *linked_list, ull key) {
    struct sll_node *node = linked_list->head;
    struct sll_node *previous = NULL;

    while(node != NULL) {
        if (node->key_value.key == key) {
            
            node->key_value.value--;
            
            if (node->key_value.value == 0) {
                /* Delete node */
                linked_list->count--;

                if (previous != NULL) {
                    previous->next = node->next;
                } else {
                    /* previous == NULL: next goes on head */
                    linked_list->head = node->next;
                }

                free(node);
            }

            /* Else: Don't do anything */

            break;
        }

        previous = node;
        node = node->next;
    }
}
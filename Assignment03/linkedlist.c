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

static struct sll_node *sll_find(struct linked_list *linked_list, ull key) {
    struct sll_node *node = linked_list->head;

    while(node != NULL) {
        if (node->key_value.key == key) {
            break;
        }
        node = node->next;
    }

    return node;
}

void sll_put_key_value(struct linked_list *linked_list, struct key_value key_value) {
    struct sll_node *new_head = (struct sll_node*) malloc(sizeof(struct sll_node));
    new_head->key_value = key_value;
    
    if (linked_list->head != NULL) {
        new_head->next = linked_list->head;
    } else {
        new_head->next = NULL;
    }

    linked_list->head = new_head;
    linked_list->count++;
}

static void sll_insert(struct linked_list *linked_list, ull key) {
    struct key_value key_value;
    key_value.key = key;
    key_value.value = 0u;
    sll_put_key_value(linked_list, key_value);
}

void sll_add(struct linked_list *linked_list, ull key) {
    struct sll_node *node = sll_find(linked_list, key);

    if (node != NULL) {
        node->key_value.value++;
    } else {
        sll_insert(linked_list, key);
    }
}

void sll_remove(struct linked_list *linked_list, ull key) {
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

bool sll_pop(struct linked_list *linked_list, struct key_value *key_value) {
    if (linked_list->head != NULL) {
        *key_value = linked_list->head->key_value;
        sll_remove(linked_list, key_value->key);
        linked_list->count--;
        return true;
    } else {
        /* Empty */
        return false;
    }
}
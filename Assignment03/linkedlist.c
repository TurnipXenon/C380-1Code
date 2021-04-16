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
    #ifdef DEBUG_PRINT
    printf("Add (%llX): %u\n", key, 1u);
    #endif /* DEBUG_PRINT */
    struct key_value key_value;
    key_value.key = key;
    key_value.value = 1u;
    sll_put_key_value(linked_list, key_value);
}

void sll_add(struct linked_list *linked_list, ull key) {
    struct sll_node *node = sll_find(linked_list, key);

    if (node != NULL) {
        node->key_value.value++;
        #ifdef DEBUG_PRINT
        printf("Add (%llX): %llu\n", node->key_value.key, node->key_value.value);
        #endif /* DEBUG_PRINT */
    } else {
        sll_insert(linked_list, key);
    }
}

static void sll_delete(struct linked_list *linked_list,
                    struct sll_node *node, 
                    struct sll_node *previous) {
    linked_list->count--;

    if (previous != NULL) {
        previous->next = node->next;
    } else {
        /* previous == NULL: next goes on head */
        linked_list->head = node->next;
    }

    free(node);
}

void sll_remove(struct linked_list *linked_list, ull key) {
    struct sll_node *node = linked_list->head;
    struct sll_node *previous = NULL;

    while(node != NULL) {
        if (node->key_value.key == key) {

            // printf("Delete before (%llX): %llu\n", node->key_value.key, node->key_value.value);
            node->key_value.value--;
            #ifdef DEBUG_PRINT
            printf("Delete after (%llX): %llu\n", node->key_value.key, node->key_value.value);
            #endif /* DEBUG_PRINT */

            if (node->key_value.value == 0) {
                sll_delete(linked_list, node, previous);
            } /* Else: Don't do anything else */

            break;
        }

        previous = node;
        node = node->next;
    }
}

bool sll_pop(struct linked_list *linked_list, struct key_value *key_value) {
    if (linked_list->head != NULL) {
        *key_value = linked_list->head->key_value;
        sll_delete(linked_list, linked_list->head, NULL); // remove head
        linked_list->count--;
        return true;
    } else {
        /* Empty */
        return false;
    }
}
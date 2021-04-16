#include "linkedlist.h"

/**
 * @brief Properly allocate and initialize a linked list
 * 
 * @return struct linked_list* 
 */
struct linked_list *new_linked_list() {
    struct linked_list *new = (struct linked_list*) malloc(sizeof(struct linked_list));
    new->count = 0;
    new->head = NULL;
    return new;
}

/**
 * @brief Properly clean up linked list
 * 
 * @param linked_list 
 */
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

/**
 * @brief Find a node with the given key, and return the node. If not found,
 * returns NULL
 * 
 * @param linked_list 
 * @param key 
 * @return struct sll_node* 
 */
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

/**
 * @brief Puts a key value pair as it is on the head
 * 
 * @param linked_list 
 * @param key_value 
 */
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

/**
 * @brief Insert a key value pair at the head
 * 
 * @param linked_list 
 * @param key 
 */
static void sll_insert(struct linked_list *linked_list, ull key) {
    #ifdef DEBUG_PRINT
    printf("Add (%llX): %u\n", key, 1u);
    #endif /* DEBUG_PRINT */
    struct key_value key_value;
    key_value.key = key;
    key_value.value = 1u;
    sll_put_key_value(linked_list, key_value);
}

/**
 * @brief Increment a value with the matching key, otherwise add a new node 
 * for the key-value pair.
 * 
 * @param linked_list 
 * @param key 
 */
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

/**
 * @brief Delete a linked list. This needs the previous node
 * to connect it's next node to.
 * 
 * @param linked_list 
 * @param node 
 * @param previous 
 */
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

/**
 * @brief Decrement a value with the matching key. If the value reaches zero, the node
 * is deleted.
 * 
 * @param linked_list 
 * @param key 
 */
void sll_remove(struct linked_list *linked_list, ull key) {
    struct sll_node *node = linked_list->head;
    struct sll_node *previous = NULL;

    while(node != NULL) {
        if (node->key_value.key == key) {

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

/**
 * @brief Remove head and set it to key_value. Return false if there's no element inside
 * the linked list.
 * 
 * @param linked_list 
 * @param key_value 
 * @return true 
 * @return false 
 */
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
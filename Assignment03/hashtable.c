#include "hashtable.h"

struct hashtable *_new_hashtable(ull level) {
    struct hashtable *hashtable = (struct hashtable*) malloc(sizeof(struct hashtable));
    hashtable->count = 0u;
    hashtable->level = level;
    hashtable->shifts = get_power_of_two(TABLE_SIZE) * level;

    for (size_t i = 0; i < TABLE_SIZE; ++i) {
        hashtable->bucket[i].table = NULL;
        hashtable->bucket[i].linked_list = new_linked_list();
    }
    
    return hashtable;
}

struct hashtable *new_hashtable() {
    return _new_hashtable(0);
}

void destroy_hashtable(struct hashtable *hashtable) {
    if (hashtable != NULL) {
        
        for (size_t i = 0; i < TABLE_SIZE; ++i) {
            if (hashtable->bucket[i].table != NULL) {
                destroy_hashtable(hashtable->bucket[i].table);
            }
            
            destroy_linked_list(hashtable->bucket[i].linked_list);
        }

        free(hashtable);
    }
}

// static struct key_value *find_in_bucket(struct key_value *bucket[], ull key) {
//     for (size_t i = 0; i < GROW_SIZE; i++)
//     {
//         if (bucket[i]->key == key) {
//             return bucket[i];
//         }
//     }
    
//     return NULL;
// }

// static void add_to_bucket(struct key_value *bucket[], ull key) {
//     struct key_value *node = (struct key_value*) malloc(sizeof(struct key_value));
//     size_t i = 0;

//     node->key = key; 
//     node->value = 1; // we now have one thing

//     for (; i < GROW_SIZE; ++i) {
//         if (bucket[i] == NULL) {
//             bucket[i] = node;
//             break;
//         }
//     }
    
//     if (i == GROW_SIZE) {
//         perror("Bucket overfilled! This should not happen. Entry not added.\n");
//     }
// }

/**
 * @brief 
 * 
 * Assumes that the key is unique.
 * 
 * @param hashtable 
 * @param key_value 
 */
static void hashtable_put_key_value(struct hashtable *hashtable, struct key_value key_value) {
    // find appropriate bucket based on table size
    ull index = (key_value.key >> hashtable->shifts) & HASH_MASK;
    struct bucket *bucket = &(hashtable->bucket[index]);
    ull old_count;

    if (bucket->table != NULL) {

        /* This bucket was already expanded */
        old_count = bucket->table->count;
        hashtable_put_key_value(bucket->table, key_value);
        if (old_count != bucket->table->count) {
            /* Change detected! Increase count */
            hashtable->count++;
        }

    } else {

        /* This bucket was not expanded */
        old_count = bucket->linked_list->count;
        sll_put_key_value(bucket->linked_list, key_value);
        if (old_count != bucket->linked_list->count) {
            /* Change detected! Increase count */
            hashtable->count++;
        }

        if (bucket->linked_list->count > SPLIT_SIZE) {

            bucket->table = _new_hashtable(hashtable->level + 1);
            struct key_value key_value;

            while(sll_pop(bucket->linked_list, &key_value)) {
                hashtable_put_key_value(bucket->table, key_value);
            }

        }

    }

    // todo: P1
}

void put(struct hashtable *hashtable, ull address) {
    // find appropriate bucket based on table size
    ull index = (address >> hashtable->shifts) & HASH_MASK;
    struct bucket *bucket = &(hashtable->bucket[index]);
    ull old_count;

    if (bucket->table != NULL) {

        /* This bucket was already expanded */
        // printf("Put in hashtable; ");
        old_count = bucket->table->count;
        put(bucket->table, address);
        if (old_count != bucket->table->count) {
            // printf("Split table; ");
            /* Change detected! Increase count */
            hashtable->count++;
        }

    } else {

        /* This bucket was not expanded */
        old_count = bucket->linked_list->count;
        // printf("Put in SLL; ");
        sll_add(bucket->linked_list, address);
        if (old_count != bucket->linked_list->count) {
            /* Change detected! Increase count */
            hashtable->count++;
        }

        if (bucket->linked_list->count > SPLIT_SIZE) {

            bucket->table = _new_hashtable(hashtable->level + 1);
            struct key_value key_value;

            while(sll_pop(bucket->linked_list, &key_value)) {
                hashtable_put_key_value(bucket->table, key_value);
            }

        }

    }

    // todo: put(struct hashtable *hashtable, struct mem_ref mem_ref)

    // todo: update size
}

void delete(struct hashtable *hashtable, ull address) {
    // find appropriate bucket based on table size
    ull index = (address >> hashtable->shifts) & HASH_MASK;
    struct bucket *bucket = &(hashtable->bucket[index]);
    ull old_count;

    if (bucket->table != NULL) {

        /* This bucket was already expanded */
        old_count = bucket->table->count;
        delete(bucket->table, address);
        if (old_count != bucket->table->count) {
            /* Change detected! Increase count */
            hashtable->count--;
        }

        if (bucket->table->count < MERGE_SIZE) {
            struct hashtable *inner_table = bucket->table;
            struct key_value key_value;

            for (size_t i = 0; i < TABLE_SIZE; ++i) {
                while(sll_pop(inner_table->bucket[i].linked_list, &key_value)) {
                    sll_put_key_value(bucket->linked_list, key_value);
                }
            }
            
            destroy_hashtable(bucket->table);
            bucket->table = NULL;

        }

    } else {

        /* This bucket was not expanded */
        old_count = bucket->linked_list->count;
        sll_remove(bucket->linked_list, address);
        if (old_count != bucket->linked_list->count) {
            /* Change detected! Increase count */
            hashtable->count--;
        }

    }

    // todo: delete(struct hashtable *hashtable, struct mem_ref mem_ref)

    // todo: update size
}
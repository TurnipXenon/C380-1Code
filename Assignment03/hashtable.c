#include "hashtable.h"


/**
 * @brief Allocate and properly initialize a hashtable for windowset
 * 
 * @param level needed for indexing and creating deeper levels
 * @return struct hashtable* 
 */
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

/**
 * @brief Allocate and properly initialize a hashtable for windowset
 * 
 * Hides the actual need for the levels for creating a new hashtable
 * 
 * @return struct hashtable* 
 */
struct hashtable *new_hashtable() {
    return _new_hashtable(0);
}

/**
 * @brief Do clean up properly for a hashtable
 * 
 * @param hashtable 
 */
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

/**
 * @brief Puts the key value pair as-is into the hashtable buckets
 * 
 * Assumes that the key is unique.
 * 
 * @param hashtable 
 * @param key_value 
 */
static void hashtable_put_key_value(struct hashtable *hashtable, struct key_value key_value) {
    /* find appropriate bucket based on table size */
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
}

/**
 * @brief Increase the count of the address on the hashtable
 * 
 * @param hashtable 
 * @param address 
 */
void put(struct hashtable *hashtable, ull address) {
    /* find appropriate bucket based on table size */
    ull index = (address >> hashtable->shifts) & HASH_MASK;
    struct bucket *bucket = &(hashtable->bucket[index]);
    ull old_count;

    if (bucket->table != NULL) {

        /* This bucket was already expanded */
        old_count = bucket->table->count;
        put(bucket->table, address);
        if (old_count != bucket->table->count) {
            /* Change detected! Increase count */
            hashtable->count++;
        }

    } else {

        /* This bucket was not expanded */
        old_count = bucket->linked_list->count;
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
}


/**
 * @brief Reduce the count of the address on the hashtable
 * 
 * @param hashtable 
 * @param address 
 */
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
}
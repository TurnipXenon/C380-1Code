#include "hashtable.h"

struct hashtable *_new_hashtable(ull level) {
    struct hashtable *hashtable = (struct hashtable*) malloc(sizeof(struct hashtable));
    hashtable->count = 0u;
    hashtable->level = level;

    for (size_t i = 0; i < TABLE_SIZE; ++i) {
        hashtable->bucket[i].table = NULL;
        hashtable->bucket[i].linked_list = new_linked_list();
    }
    
    return hashtable;
}

struct hashtable *new_hashtable(ull level) {
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
        hashtable = NULL;

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

void put(struct hashtable *hashtable, ull address) {
    switch (hashtable->mode) {
        case SINGLE:
            // find in the buckets
            struct key_value *entry = find_in_bucket(hashtable->entries, address);

            if (entry != NULL) {
                /* Entry found: just increment value. Size of pages don't change */
                entry->value++;
                return;
            }

            /* Entry not found: entry == NULL, try adding to bucket */
            add_to_bucket(hashtable->entries, address);
            hashtable->count++;

            if (hashtable->count >= GROW_SIZE) {
                hashtable->mode = DUAL;

                // find lowest

                // what's my hash function anyway
            }

                // if not found, add to bucket
            // if bucket full, switch to dual mode + split
            break;
        
        case DUAL:
            // find >= bucket

            // update the size of the two buckets
            break;

        default:
            printf("Unknown hashtable mode: %d\n", hashtable->mode);
            break;
    }

    // todo: put(struct hashtable *hashtable, struct mem_ref mem_ref)

    // todo: update size
}

void delete(struct hashtable *hashtable, ull address) {
    switch (hashtable->mode) {
        case SINGLE:
            // find in the buckets
            break;
        
        case DUAL:
            // find >= bucket

            // if the size of the two buckets under shrink size, switch to single mode
            break;

        default:
            printf("Unknown hashtable mode: %d\n", hashtable->mode);
            break;
    }

    // todo: delete(struct hashtable *hashtable, struct mem_ref mem_ref)

    // todo: update size
}
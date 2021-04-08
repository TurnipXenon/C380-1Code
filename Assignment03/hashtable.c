#include "hashtable.h"

struct hashtable *new_hashtable() {
    struct hashtable *root = (struct hashtable*) malloc(sizeof(struct hashtable));
    root->mode = SINGLE;
    root->count = 0u;
    root->entry_nodes = NULL;
    root->upper = NULL;
    root->lower = NULL;
    return root;
}

void destroy_hashtable(struct hashtable *hashtable) {
    // todo: clean up
    if (hashtable != NULL) {
        free(hashtable);
        hashtable = NULL;
    }
}


void put(struct hashtable *hashtable, ull address) {
    switch (hashtable->mode) {
        case SINGLE:
            // find in the buckets

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
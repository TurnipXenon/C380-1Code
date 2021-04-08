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


ull put(struct hashtable *hashtable, struct mem_ref mem_ref) {
    switch (hashtable->mode) {
        case SINGLE:
            // find in the buckets
            break;
        
        case DUAL:
            break;

        default:
            perror("Unknown hashtable mode: %d\n", hashtable->mode);
            break;
    }

    if (hashtable->count <= GROW_SIZE) {
        /* the files are just local in entry nodes */
        hashtable->count++;

        if (hashtable->count >= GROW_SIZE) {
            /* todo: split*/
        } else {
            /* todo: put in bucket */
        }
    } else {
        /* the files are in the "buckets" */
    }

    // todo: put(struct hashtable *hashtable, struct mem_ref mem_ref)

    // todo: update size
}

ull delete(struct hashtable *hashtable, struct mem_ref mem_ref) {
    // todo: delete(struct hashtable *hashtable, struct mem_ref mem_ref)

    // todo: update size
}
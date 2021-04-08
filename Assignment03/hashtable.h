#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include "valws379_base.h"

#define GROW_SIZE 10
#define SHRINK_SIZE 4

struct entry {
    ull key;
    ull value;
};

struct entry_node {
    struct entry entry;
    struct entry_node* next;
};

enum hashtable_mode {
    SINGLE,
    DUAL
};

struct hashtable {
    enum hashtable_mode mode;
    ull count;
    ull height;
    struct entry_node *entry_nodes;
    struct hashtable *upper;
    struct hashtable *lower;
};

struct hashtable *new_hashtable();

void destroy_hashtable(struct hashtable *hashtable);

void put(struct hashtable *hashtable, ull address);

void delete(struct hashtable *hashtable, ull address);

#endif /* _HASHTABLE_H_ */
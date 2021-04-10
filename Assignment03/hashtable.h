#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include "valws379_base.h"
#include "linkedlist.h"

enum hashtable_mode {
    SINGLE,
    DUAL
};

struct bucket {
    struct hashtable *table; // basis of expanded or not?
    struct linked_list *linked_list;
};

struct hashtable {
    ull count;
    ull level;
    ull shifts;
    struct bucket bucket[TABLE_SIZE];
};

struct hashtable *new_hashtable();

void destroy_hashtable(struct hashtable *hashtable);

void put(struct hashtable *hashtable, ull address);

void delete(struct hashtable *hashtable, ull address);

#endif /* _HASHTABLE_H_ */
/**
 * @file hashtable.h
 * @brief Hashtable that uses the least significant bits and some shifting based on the
 * how deep it is in the level for indexing.
 */

#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include "valws379_base.h"
#include "linkedlist.h"

/**
 * @brief Acts as either a hashtable or linked list depending on how much data is inside
 */
struct bucket {
    struct hashtable *table; // basis of expansion
    struct linked_list *linked_list;
};

/**
 * @brief Special hashtable for windowset
 */
struct hashtable {
    ull count;
    ull level;
    ull shifts;
    struct bucket bucket[TABLE_SIZE];
};

/**
 * @brief Allocate and properly initialize a hashtable for windowset
 * 
 * @return struct hashtable* 
 */
struct hashtable *new_hashtable();

/**
 * @brief Do clean up properly for a hashtable
 * 
 * @param hashtable 
 */
void destroy_hashtable(struct hashtable *hashtable);

/**
 * @brief Increase the count of the address on the hashtable
 * 
 * @param hashtable 
 * @param address 
 */
void put(struct hashtable *hashtable, ull address);

/**
 * @brief Reduce the count of the address on the hashtable
 * 
 * @param hashtable 
 * @param address 
 */
void delete(struct hashtable *hashtable, ull address);

#endif /* _HASHTABLE_H_ */
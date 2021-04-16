#ifndef _VALWS_379_H_
#define _VALWS_379_H_

#include <stdio.h> /* perror */
#include <stddef.h> /* for NULL */
#include <stdlib.h> /* for malloc */
#include <stdbool.h>

#define TABLE_SIZE 16
#define HASH_MASK (TABLE_SIZE - 1)
#define SPLIT_SIZE 15
#define MERGE_SIZE 6

typedef unsigned long long ull;

struct mem_ref {
    ull address;
    ull page_count;
};

struct key_value {
    ull key;
    ull value;
};

struct mem_ref new_mem_ref();

/**
 * @brief Get the power of two for a value.
 * Assumes that value is > 0
 * 
 * @param value 
 * @return ull 
 */
ull get_power_of_two(ull value);

#endif /* _VALWS_379_H_ */
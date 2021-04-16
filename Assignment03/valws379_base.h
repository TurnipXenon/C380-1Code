/**
 * @file valws379_base.h
 * @brief Base for all other files
 */

#ifndef _VALWS_379_H_
#define _VALWS_379_H_

#include <stdio.h> /* perror */
#include <stddef.h> /* for NULL */
#include <stdlib.h> /* for malloc */
#include <unistd.h> /* for getopt */
#include <libgen.h> /* for basename */
#include <ctype.h> /* for isspace */
#include <string.h> /* strcmp */
#include <stdbool.h>

#define TABLE_SIZE 16

/* Used for getting the least significant digits in hashtable */
#define HASH_MASK (TABLE_SIZE - 1) 

#define SPLIT_SIZE 15
#define MERGE_SIZE 6

typedef unsigned long long ull;

/**
 * @brief Processed memory reference for the queue
 */
struct mem_ref {
    ull address;
    ull page_count;
};

/**
 * @brief Key-value pair for hashtable
 */
struct key_value {
    ull key;
    ull value;
};

/**
 * @brief Create new mem_ref
 * 
 * @return struct mem_ref 
 */
struct mem_ref new_mem_ref();

/**
 * @brief Get the power of two for a value.
 * Assumes that value is > 0
 * 
 * @param value 
 * @return ull 
 */
ull get_power_of_two(ull value);

/**
 * @brief Print usage instruction
 * 
 * @remark from Assignment 2 getopt example
 * 
 * @param program_name or argv[0]
 */
void print_usage(char *program_name);

/**
 * @brief Checks if line is empty
 * 
 * @remark from https://stackoverflow.com/a/27607770/10024566
 * 
 * @param line 
 * @return bool 
 */
bool is_empty_line(const char *line);

/**
 * @brief Checks if key is either I, S, M, or L.
 * 
 * @param key 
 * @return true
 * @return false 
 */
bool is_valid_code(const char* key);

#endif /* _VALWS_379_H_ */
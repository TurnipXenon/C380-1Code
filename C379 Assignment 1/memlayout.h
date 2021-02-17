/**
 * @file memlayout.h
 * @author Allan Manuba
 * 
 * @brief Scans the entire memory area of the calling process
 * 
 * Some of the documentation texts are paraphrased from the Assignment instruction
 * and the lab slides.
 */

#ifndef MEM_LAYOUT_H_
#define MEM_LAYOUT_H_

#define MEM_RW 0 /**< @brief memory read / write accessibility */
#define MEM_RO 1 /**< @brief memory read-only accessibility */
#define MEM_NO 2 /**< @brief no accessbility */

#include <stdio.h> // for printing
#include <setjmp.h>
#include <stdlib.h> // for malloc
#include <signal.h>
#include <inttypes.h> // for printing proper looking hex codes
#include <stdbool.h>

/** 
 * @brief Smallest memory space unit assumed by get_mem_layout() with same accessibility
 * 
 * Modify PAGE_SIZE by setting by defining USER_PAGE_SIZE in CFLAGS.
 */

#ifndef PPPP
#define PPPP
const unsigned int PAGE_SIZE = 4096;
#endif

/**
 * @brief Stores context during sigsetjumps in @see get_mem_layout
 */
sigjmp_buf env;

/**
 * @struct memregion
 * @brief memregion or memory region is a contiguous area of memory 
 * characterized by the same accessibility
 */
struct memregion
{
    void *from; /**< @brief from is the starting address of the memory region */
    void *to; /**< @brief to is the last address of the memory region */

    /** 
     * @brief mode is the type of memory accessibility for the memory region,
     * which can be MEM_RW, or MEM_RO, or MEM_NO 
     */
    unsigned char mode;
};

/**
 * @brief Returns how many memory regions there are in the process's address space
 * and lists them in @p regions
 * 
 * If there are more memory regions than the given @p size, we only fill up
 * first @p size elements @p regions. Check out @see memregions for more details about
 * what memory regions are.
 * 
 * We assume that @p regions is empty, or the user knows that we did not clear the rest
 * of @p regions for them.
 * 
 * @param regions list of memory regions that the function may modify
 * @param size size of the list @p regions
 * @return int count of how many memory regions there are in the process's address space
 */
int get_mem_layout(struct memregion *regions, unsigned int size);

/**
 * @brief Handles the segfault exception in get_mem_layout
 * 
 * @param sig signal number being handled by the function
 */
void handle_signal(int sig);

/**
 * @brief Prints each memregion in a readable manner 
 * 
 * Each memory region in @p regions is formatted as "0xAAAAAAAA - 0xBBBBBBBB CC"
 * where AAAAAAAA is the hexadecimal value for the start address in the memory region,
 * BBBBBBBB is the hexadecimal value for the last address in the memory region,
 * and CC represents memory accessibility which can either be "NO", "RO", or "RW".
 * 
 * @param regions list of @see memregion
 * @param size size of the list @p regions
 * @param region_count the actual account of memory region based on @see get_mem_layout
 */
void print_mem_layout(struct memregion *regions, unsigned int size, int region_count);

/**
 * @brief converts the @see memregion#mode into const char*
 * 
 * @param mode memory accessibility in @see memregion#mode
 * @return const char* 
 */
const char* mode_to_string(unsigned char mode);

#endif /* MEM_LAYOUT_H_ */
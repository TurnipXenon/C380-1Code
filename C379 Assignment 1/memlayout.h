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

#define MEM_RW 0
#define MEM_RO 1
#define MEM_NO 2

#include <stdio.h> // for printing
#include <setjmp.h>
#include <stdlib.h> // for malloc
#include <signal.h>
#include <inttypes.h> // for printing proper looking hex codes

/** 
 * @brief Smallest memory space unit assumed by get_mem_layout() with same accessibility
 * 
 * Modify PAGE_SIZE by setting by defining USER_PAGE_SIZE in CFLAGS.
 */
extern const unsigned int PAGE_SIZE;

/**
 * @struct memregion
 * @brief memregion is a contiguous area of memory characterized by the same accessibility
 */
struct memregion
{
    void *from; /**< @brief from is the starting address of the memregion */
    void *to; /**< @brief to is the last address of the memregion */

    /** 
     * @brief mode is the type of memory accessibility for the memregion,
     * which can be MEM_RW, or MEM_RO, or MEM_NO 
     */
    unsigned char mode;
};

/**
 * @brief handles the segfault exception in get_mem_layout
 * 
 * @param sig 
 */
void handle_segfault(int sig);

/**
 * @brief Get the mem layout object
 * 
 * @param regions 
 * @param size 
 * @return int 
 */
int get_mem_layout(struct memregion *regions, unsigned int size);

/* todo: memlayout.h also has a definition for a const unsigned 
int PAGE_SIZE. Usingpre-processor directives memlayout.h should 
check if USER_PAGE_SIZE is set and to use it as PAGE_SIZE,and 
if not defined to set PAGE_SIZE to 4096. */

/**
 * @brief 
 * 
 * @param regions 
 * @param size 
 * @param region_count 
 */
void print_mem_layout(struct memregion *regions, unsigned int size, int region_count);

const char* mode_to_string(unsigned char mode);

#endif /* MEM_LAYOUT_H_ */
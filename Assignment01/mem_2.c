#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include "memlayout.h"

/**
 * @brief Driver mmaps a large file at a specific address (from Assignment description)
 */
int main() {
    const unsigned int SIZE = 30;
    struct memregion regions_before[SIZE];
    struct memregion regions_after[SIZE];
    int regions_before_count, regions_after_count;

    regions_before_count = get_mem_layout(regions_before, SIZE);
    
    mmap(
        (void*)0x70000000u, 
        10000 * sizeof(int),
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        0, 
        0
    );
    
    regions_after_count = get_mem_layout(regions_after, SIZE);

    printf("Before (%d regions):\n", regions_before_count);
    print_mem_layout(regions_before, SIZE, regions_before_count);
    printf("After (%d regions):\n", regions_after_count);
    print_mem_layout(regions_after, SIZE, regions_after_count);

    return 0;
}
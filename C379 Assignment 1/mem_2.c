#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include "memlayout.h"

/**
 * @brief Driver which mmaps a large file at a certain address (from Assignment description)
 */
int main() {
    const unsigned int SIZE = 30;
    struct memregion regions[SIZE];
    int region_count = 0;

    region_count = get_mem_layout(regions, SIZE);
    printf("Before (%d regions):\n", region_count);
    print_mem_layout(regions, SIZE, region_count);
    
    mmap(
        (void*)0x70000000u, 
        10000 * sizeof(int),
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        0, 
        0
    );
    
    region_count = get_mem_layout(regions, SIZE);
    printf("After (%d regions):\n", region_count);
    print_mem_layout(regions, SIZE, region_count);

    return 0;
}
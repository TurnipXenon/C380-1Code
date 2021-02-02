#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include "memlayout.h"


/* todo: better document */

const char* mode_to_string(unsigned char mode) {
    switch(mode) {
        case MEM_RW:
            return "RW";
        case MEM_RO:
            return "RO";
        case MEM_NO:
            return "NO";
        default:
            return "Unknown";
    }
}

/**
 * mmap()-ing a large file
 */
int main() {
    unsigned int size = 30;
    struct memregion regions[size];
    int region_count = 0;

    region_count = get_mem_layout(regions, size);
    printf("Before (%d regions):\n", region_count);
    print_mem_layout(regions, size, region_count);
    
    int *p = mmap(0x70000000, 10000 * sizeof(int),
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        0, 0);
    
    region_count = get_mem_layout(regions, size);
    printf("After (%d regions):\n", region_count);
    print_mem_layout(regions, size, region_count);

    return 0;
}
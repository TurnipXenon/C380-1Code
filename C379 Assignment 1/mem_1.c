#include <stdlib.h>
#include <stdio.h>
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
 * Allocate a massive array with malloc and initialize it
 */
int main() {
    unsigned int size = 30;
    struct memregion regions[size];
    int region_count = 0;

    region_count = get_mem_layout(regions, size);
    printf("Before (%d regions):\n", region_count);
    print_mem_layout(regions, size, region_count);
    
    int *large_array = (int*) malloc(sizeof(int) * 1000000);
    
    region_count = get_mem_layout(regions, size);
    printf("After (%d regions):\n", region_count);
    print_mem_layout(regions, size, region_count);

    free(large_array);

    return 0;
}
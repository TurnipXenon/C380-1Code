#include <stdlib.h>
#include <stdio.h>
#include "memlayout.h"

/* todo: better document */

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
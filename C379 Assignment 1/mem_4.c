#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include "memlayout.h"

int recursive_function(struct memregion *regions, unsigned int size, int recursive_count) {
    int big_list[1000];
    
    if (recursive_count <= 0) {
        int i = get_mem_layout(regions, size);
        return i;
    } else {
        return recursive_function(regions, size, --recursive_count);
    }
}

/**
 * @brief Driver which performs a mathematical function using the standard math library, 
 * assuming always you compile dynamically loaded executables (from Assignment description)
 */
int main() {
    const int RECURSION_DEPTH = 1000;
    const unsigned int SIZE = 30;
    struct memregion regions_before[SIZE];
    struct memregion regions_after[SIZE];
    int regions_before_count, regions_after_count;

    regions_before_count = get_mem_layout(regions_before, SIZE);
    
    regions_after_count = recursive_function(regions_after, SIZE, RECURSION_DEPTH);

    printf("Before (%d regions):\n", regions_before_count);
    print_mem_layout(regions_before, SIZE, regions_before_count);
    printf("After (%d regions):\n", regions_after_count);
    print_mem_layout(regions_after, SIZE, regions_after_count);

    return 0;
}
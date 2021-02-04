#include <stdlib.h>
#include <stdio.h>
#include "memlayout.h"

/**
 * @brief  Driver which allocates a massive array with malloc and initializes it
 * (from Assignment description)
 */
int main() {
    const int LARGE_ARRAY_SIZE = 1000000;
    const unsigned int SIZE = 30;
    struct memregion regions_before[SIZE];
    struct memregion regions_after[SIZE];
    int regions_before_count, regions_after_count;

    regions_before_count = get_mem_layout(regions_before, SIZE);

    int *large_array = (int*) malloc(sizeof(int) * LARGE_ARRAY_SIZE);
    for (int i = 0; i < LARGE_ARRAY_SIZE; ++i) {
        large_array[i] = i;
    }
    
    regions_after_count = get_mem_layout(regions_after, SIZE);

    printf("Before (%d regions):\n", regions_before_count);
    print_mem_layout(regions_before, SIZE, regions_before_count);
    printf("After (%d regions):\n", regions_after_count);
    print_mem_layout(regions_after, SIZE, regions_after_count);

    free(large_array);

    return 0;
}
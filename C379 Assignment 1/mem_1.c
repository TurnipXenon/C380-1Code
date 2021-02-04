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
    struct memregion regions[SIZE];
    int region_count = 0;

    region_count = get_mem_layout(regions, SIZE);
    printf("Before (%d regions):\n", region_count);
    print_mem_layout(regions, SIZE, region_count);

    int *large_array = (int*) malloc(sizeof(int) * LARGE_ARRAY_SIZE);
    for (int i = 0; i < LARGE_ARRAY_SIZE; ++i) {
        large_array[i] = i;
    }
    
    region_count = get_mem_layout(regions, SIZE);
    printf("After (%d regions):\n", region_count);
    print_mem_layout(regions, SIZE, region_count);

    free(large_array);

    return 0;
}
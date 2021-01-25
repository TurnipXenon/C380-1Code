#include <stdlib.h>
#include "memlayout.h"

/* todo: better document */

void print_mem_layout(struct memregion *regions, unsigned int size) {
    if (regions == NULL) {
        return;
    }

    // todo: print memlayout
    for(unsigned int i = 0; i < size; ++i) {
        printf("%p\n", regions[i].from);
    }
}

/**
 * Allocate a massive array with malloc and initialize it
 */
int main() {
    unsigned int size = 10;
    struct memregion *regions = (struct memregion*) malloc(size * sizeof(struct memregion));

    printf("mem1: %p\n", regions);
    // get_mem_layout(&regions, size);
    
    printf("Stacking time\n");
    int *large_array = (int*) malloc(sizeof(int) * 1000000);
    
    // todo: do another get mem layout
    int region_count = get_mem_layout(&regions, size);
    printf("Done get mem layout\n");
    // print_mem_layout(&regions, size);

    printf("mem1: %p\n", regions);
    free(large_array);
    free(regions);

    return 0;
}
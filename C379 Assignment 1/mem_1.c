#include <stdlib.h>
#include "memlayout.h"

/* todo: better document */

/**
 * Allocate a massive array with malloc and initialize it
 */
int main(int argc, char *argv[]) {
    struct memregion *m = NULL;
    get_mem_layout(&m, 0);
    
    int *large_array = (int*) malloc(sizeof(int) * 10000);
    
    // todo: do another get mem layout

    free(large_array);
    free(m);

    return 0;
}
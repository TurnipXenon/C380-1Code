#include <stdlib.h>
#include "memlayout.h"

/* todo: better document */

/**
 * Allocate a massive array with malloc and initialize it
 */
int main(int argc, char *argv[]) {
    int *large_array = (int*) malloc(sizeof(int) * 10000);
    free(large_array);
    return 0;
}
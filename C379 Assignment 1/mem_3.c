#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include "memlayout.h"

/* todo: better document */

/**
 * Load math library
 */
int main() {
    unsigned int size = 30;
    struct memregion regions[size];
    int region_count = 0;

    region_count = get_mem_layout(regions, size);
    printf("Before (%d regions):\n", region_count);
    print_mem_layout(regions, size, region_count);
    
    // from lab slides
    double (*cosine)(double);
    void *handle = dlopen("/usr/lib32/libm.so", RTLD_LAZY);
    *(void **) (&cosine) = dlsym(handle, "cos");
    printf("%f\n", (*cosine)(2.0));
    dlclose(handle);
    
    region_count = get_mem_layout(regions, size);
    printf("After (%d regions):\n", region_count);
    print_mem_layout(regions, size, region_count);

    return 0;
}
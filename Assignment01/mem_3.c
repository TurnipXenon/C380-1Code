#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include "memlayout.h"

/**
 * @brief Driver which performs a mathematical function using the standard math library, 
 * assuming always you compile dynamically loaded executables (from Assignment description)
 */
int main() {
    const unsigned int SIZE = 30;
    struct memregion regions_before[SIZE];
    struct memregion regions_after[SIZE];
    int regions_before_count, regions_after_count;

    regions_before_count = get_mem_layout(regions_before, SIZE);
    
    // from lab slides
    double (*cosine)(double);
    void *handle = dlopen("/usr/lib32/libm.so", RTLD_LAZY);
    *(void **) (&cosine) = dlsym(handle, "cos");
    (*cosine)(2.0); // we're not printing it because there may be side effects with printing
    dlclose(handle);
    
    regions_after_count = get_mem_layout(regions_after, SIZE);

    printf("Before (%d regions):\n", regions_before_count);
    print_mem_layout(regions_before, SIZE, regions_before_count);
    printf("After (%d regions):\n", regions_after_count);
    print_mem_layout(regions_after, SIZE, regions_after_count);

    return 0;
}
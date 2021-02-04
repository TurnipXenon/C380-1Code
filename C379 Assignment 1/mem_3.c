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
    struct memregion regions[SIZE];
    int region_count = 0;

    region_count = get_mem_layout(regions, SIZE);
    printf("Before (%d regions):\n", region_count);
    print_mem_layout(regions, SIZE, region_count);
    
    // from lab slides
    double (*cosine)(double);
    void *handle = dlopen("/usr/lib32/libm.so", RTLD_LAZY);
    *(void **) (&cosine) = dlsym(handle, "cos");
    printf("%f\n", (*cosine)(2.0));
    dlclose(handle);
    
    region_count = get_mem_layout(regions, SIZE);
    printf("After (%d regions):\n", region_count);
    print_mem_layout(regions, SIZE, region_count);

    return 0;
}
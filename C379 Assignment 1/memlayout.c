/* todo: implement */

#include "memlayout.h"

#ifdef USER_PAGE_SIZE
const unsigned int PAGE_SIZE = USER_PAGE_SIZE;
#else
const unsigned int PAGE_SIZE = 4096;
#endif

int get_mem_layout(struct memregion *regions, unsigned int size) {
    return 0;
}
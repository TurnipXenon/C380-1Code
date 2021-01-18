#ifndef MEM_LAYOUT_H_
#define MEM_LAYOUT_H_

#define MEM_RW 0
#define MEM_RO 1
#define MEM_NO 2

extern const unsigned int PAGE_SIZE;

struct memregion {
    void *from;
    void *to;
    unsigned char mode; /* MEM_RW, or MEM_RO, or MEM_NO */
};

int get_mem_layout(struct memregion *regions, unsigned int size);

/* todo: memlayout.h also has a definition for a const unsigned 
int PAGE_SIZE. Usingpre-processor directives memlayout.h should 
check if USER_PAGE_SIZE is set and to use it as PAGE_SIZE,and 
if not defined to set PAGE_SIZE to 4096. */

#endif /* MEM_LAYOUT_H_ */
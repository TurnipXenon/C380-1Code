/* todo: implement */

#include "memlayout.h"

#ifdef USER_PAGE_SIZE
const unsigned int PAGE_SIZE = USER_PAGE_SIZE;
#else
const unsigned int PAGE_SIZE = 4096;
#endif

sigjmp_buf env;

void handle_segfault(int sig) {
    printf("OUCH – I got signal %d\n", sig);  
    (void) signal(SIGSEGV, SIG_IGN);
    siglongjmp(env, 1);
}

int get_mem_layout(struct memregion *regions, unsigned int size) {
    (void) signal(SIGSEGV, handle_segfault);

    int i;
    i = sigsetjmp(env, 1);
    printf("i = %d\n", i);

    if (i == 0) {
        char *p = (char*) 0x00000000;
        char p1 = p[0];
    }

    printf("Tomatoes\n");

    return 0;
}
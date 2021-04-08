#ifndef _VALWS_379_H_
#define _VALWS_379_H_

#include <stdio.h> /* perror */
#include <stddef.h> /* for NULL */
#include <stdlib.h> /* for malloc */

typedef unsigned long long ull;

struct mem_ref {
    ull address;
    ull page_count;
};

struct mem_ref new_mem_ref();

#endif /* _VALWS_379_H_ */
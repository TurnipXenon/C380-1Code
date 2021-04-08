#ifndef _VALWS_379_H_
#define _VALWS_379_H_

#include <stddef.h> /* for NULL */
#include <stdlib.h> /* for malloc */

typedef unsigned long long ull;

struct mem_ref {
    ull address;
    ull page_size;
};

struct mem_ref new_mem_ref();

#endif /* _VALWS_379_H_ */
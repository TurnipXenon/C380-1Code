#ifndef _VALWS_379_H_
#define _VALWS_379_H_

#include <stddef.h> /* for NULL */

typedef unsigned long long ull;

struct mem_ref {
    ull address;
    ull page_size;
};

struct mem_ref create_mem_ref();

#endif /* _VALWS_379_H_ */
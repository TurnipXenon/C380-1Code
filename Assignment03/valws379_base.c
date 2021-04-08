#include "valws379_base.h"

struct mem_ref new_mem_ref() {
    struct mem_ref mem_ref;
    mem_ref.address = 0u;
    mem_ref.page_count = 0u;
    return mem_ref;
}
#include "valws379_base.h"

struct mem_ref new_mem_ref() {
    struct mem_ref mem_ref;
    mem_ref.address = 0u;
    mem_ref.page_count = 0u;
    return mem_ref;
}

/**
 * @brief Get the power of two for a value.
 * Assumes that value is > 0
 * 
 * @param value 
 * @return ull 
 */
ull get_power_of_two(ull value) {
    ull exponent = -1;
    ull new_value = value;
    
    while (new_value != 0) {
        ++exponent;
        new_value = new_value >> 1;
    }

    return exponent;
}
#include "windowset.h"

static ull page_size = 16;
static ull window_size = 100;
static ull set_size = 0;

/**
 * @brief Set the page size
 * 
 * @param value ull
 */
void set_page_size(ull value) {
    page_size = value;
}

/**
 * @brief Set the window size object
 * 
 * @param windows_size ull
 */
void set_window_size(ull value) {
    window_size = value;
}

/**
 * @brief Insert new memory reference into window set
 * 
 * This automatically purges old memory references
 * 
 * @param address 
 * @param page_size 
 */
void window_set_insert(ull address, ull page_size) {
    // todo: window_set_insert(ull address, ull page_size)
}

/**
 * @brief Get the size of the window set
 * 
 * @return ull 
 */
ull get_size() {
    return set_size;
}
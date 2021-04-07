#include "windowset.h"

static int page_size = 16;
static unsigned long window_size = 100;
static ull set_size = 0;

/**
 * @brief Set the page size
 * 
 * @param value int
 */
void set_page_size(int value) {
    page_size = value;
}

/**
 * @brief Set the window size object
 * 
 * @param windows_size unsigned long
 */
void set_window_size(unsigned long value) {
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
void insert(ull address, ull page_size) {
    // todo: insert(ull address, ull page_size)
}

/**
 * @brief Get the size of the window set
 * 
 * @return ull 
 */
ull get_size() {
    return set_size;
}
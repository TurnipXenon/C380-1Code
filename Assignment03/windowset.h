/**
 * @file windowset.h
 * @author Allan Manuba
 * @brief 
 * @date 2021-04-07
 * 
 * @remark Call set_page_size and set_window_size before using window_set_insert even if 
 * there are default values for them.
 * @remark define DEBUG_PRINT to access windowset_debug
 * 
 */

#ifndef _WINDOW_SET_H_
#define _WINDOW_SET_H_

#include "valws379_base.h"
#include "queue.h"
#include "hashtable.h"

/**
 * @brief 
 * @brief Initialize window set
 * 
 * Initialize everything required for the window set to work.
 * 
 * @param page_size_ 
 * @param window_size_ 
 */
void initialize_window_set(ull page_size_, ull window_size_);

/**
 * @brief Insert new memory reference into window set
 * 
 * This automatically purges old memory references
 * 
 * @param address 
 * @param page_size 
 */
void window_set_insert(ull address, ull page_size);

/**
 * @brief Get the size of the window set
 * 
 * @return ull 
 */
ull get_window_set_size();

/**
 * @brief Clean up
 */
void destroy_window_set();

#if DEBUG_PRINT
/**
 * @brief Print details about the queue for debugging purposes
 */
void window_set_debug();
#endif /* DEBUG_PRINT */

#endif /* _WINDOW_SET_H_ */
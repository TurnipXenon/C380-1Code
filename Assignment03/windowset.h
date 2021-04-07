/**
 * @file windowset.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-04-07
 * 
 * @remark
 * Call set_page_size and set_window_size before using insert even if there are default values
 * for them.
 */

#ifndef _WINDOW_SET_H_
#define _WINDOW_SET_H_

typedef unsigned long long ull;

/**
 * @brief Set the page size
 * 
 * This is used to better determine the load factor for the hashtable
 * 
 * @param value int
 */
void set_page_size(int value);

/**
 * @brief Set the window size object
 * 
 * This is used to know which addresses to purge when @ref insert(ull) is used
 * 
 * @param windows_size unsigned long
 */
void set_window_size(unsigned long value);

/**
 * @brief Insert new memory reference into window set
 * 
 * This automatically purges old memory references
 * 
 * @param address 
 * @param page_size 
 */
void insert(ull address, ull page_size);

/**
 * @brief Get the size of the window set
 * 
 * @return ull 
 */
ull get_size();

#endif /* _WINDOW_SET_H_ */
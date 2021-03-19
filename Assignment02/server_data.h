#ifndef _SERVER_DATA_H_
#define _SERVER_DATA_H_

#include <pthread.h>
#include <sys/time.h>
#include <stdbool.h>
#include <sched.h>

#include "notapp_base.h"

#define CLIENT_MAX 256

/**
 * @brief Keeps strings to be sent to user clients with metadata for sorting purposes
 */
struct user_entry {
    bool is_taken;
    struct timeval tv;
    char string[BUF_SIZE];
};

#define USER_ENTRY_SIZE (sizeof (user_entry))

/**
 * @brief Who should have priority to read and write shifts per stat
 * 
 * DONE: observers take turn writing on the shared data
 * sorter waits for server timer to finish so they can flip the state
 * users wait
 * 
 * SORTING: observers must wait 
 * sorter sorts the array, then they switch the state
 * users wait
 * 
 * READING: observers must wait
 * sorters wait
 * users will now take turns reading the data, the last user should reset the array size
 */
enum server_state {
    DONE,
    SORTING,
    READING
};

/**
 * @brief Determines when the state transitions from DONE to SORTING
 * 
 * This should only be used by server_timer and output_sorter
 */
void set_timer_expired(bool status);

/**
 * @brief Checks if timer has expired
 * 
 * @return true 
 * @return false 
 */
bool get_timer_expired();

/**
 * @brief Checks if a lock was detected
 * 
 * A lock occurs when output_sorter gets stuck, and timer has already expired again.
 * 
 * @return true 
 * @return false 
 */
bool is_locked();



/**
 * @brief Set the server state
 * 
 * This should only be used by output_sorter
 * 
 * @param server_state 
 */
void set_server_state(enum server_state server_state);

/**
 * @brief Get the server state object
 * 
 * @return enum server_state 
 */
enum server_state get_server_state();

/**
 * @brief Increase connected user client count
 */
void increment_user_count();

/**
 * @brief Decrease connected user client count
 */
void decrement_user_count();



/**
 * @brief Get how many reading user threads there are
 * 
 * @return int 
 * READERS can't really access until the server state is at reading no need for a mutex
 */
int get_reading_user_count();

/**
 * @brief Checks how many user threads are done sending all their entries
 * 
 * @return true 
 * @return false 
 */
bool are_readers_done();

/**
 * @brief Resets the count of readers done sending for the next round of sending
 */
void reset_reader_done_count();

/**
 * @brief Register a reader thread, equivalent to increasing the reader count
 */
void register_user();

/**
 * @brief Unregister a reader thread, equivalent to decreasing the reader count
 */
void unregister_user();

/**
 * @brief Get how many observer threads there are accessing the entry list
 * 
 * @return int writer index used for add_entry()
 */
int get_entry_writer_count();

/**
 * @brief Register an observer_thread, equivalent to increasing the writer count
 * 
 * @return int 
 */
int register_observer();

/**
 * @brief Register an observer_thread, equivalent to increasing the writer count
 */
void unregister_observer(int index);

/**
 * @brief Sort entries in entry array
 * 
 * Qsort reference from https://www.tutorialspoint.com/c_standard_library/c_function_qsort.htm
 */
void sort_entry_array();

/**
 * @brief Adds an entry into the entry array
 * 
 * @param struct user_entry
 * @param int writer index where to overwrite the entry
 */
void add_entry(struct user_entry *entry, int index);

/**
 * @brief Send all entries to a user client by sending how many entries there are followed
 * by all the entries using send_string(). Receiver are expected to use read_string().
 * 
 * @param socket 
 */
void send_entries(int socket);

/**
 * @brief Prints the logfile.
 * 
 * @param logfile
 */
void print_logfile(char *logfile);

#endif /* _SERVER_DATA_H_ */
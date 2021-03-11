#ifndef _SERVER_DATA_H_
#define _SERVER_DATA_H_

#include <pthread.h>
#include <sys/time.h>
#include <stdbool.h>
#include <sched.h>

#include "notapp_base.h"

#define CLIENT_MAX 256

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

/* todo: delete */
void test_stub();

/**
 * @brief Determines when the state transitions to SORTING
 * 
 * This is only accessed by server_timer and output_sorter
 */
void set_timer_expired(bool status);
bool get_timer_expired();

enum server_state get_server_state();
void set_server_state(enum server_state server_state);

void increment_user_count();
void decrement_user_count();

int are_readers_done();
int get_reading_user_count();
void reset_reader_done_count();
void increment_reading_user_count();
void decrement_reading_user_count();
void register_reader();
void unregister_reader();

/**
 * @brief Get the writing state object
 * 
 * @return
 */
int get_entry_writer_count();
int register_writer();
void unregister_writer(int index);
void reset_entry_array();
void sort_entry_array();
void send_entries(int socket);
/* https://www.tutorialspoint.com/c_standard_library/c_function_qsort.htm */
void output_entry_sorter();
void add_entry(struct user_entry *entry, int index);

#endif /* _SERVER_DATA_H_ */
#ifndef _SERVER_H_
#define _SERVER_H_

#include <sys/time.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h> /* To get host ip */
#include <pthread.h>
#include <sys/inotify.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#include "notapp_base.h"
#include "socket_helper.h"

#define CLIENT_MAX 256

typedef struct thread_arg {
    int sock;
    int interval;
} thread_arg;

#define THREAD_ARG_SIZE (sizeof (thread_arg))

struct user_entry {
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

/* region shared data */

/**
 * @brief Determines when the state transitions to SORTING
 * 
 * This is only accessed by server_timer and output_sorter
 */
extern bool timer_expired;
extern pthread_mutex_t timer_expired_mutex;
void set_timer_expired(bool status);

extern enum server_state server_state;
extern pthread_mutex_t server_state_mutex;
void set_server_state(enum server_state server_state);

extern int user_count;
extern pthread_mutex_t user_count_mutex;
void increment_user_count();
void decrement_user_count();

extern int reading_user_count;
extern pthread_mutex_t reading_user_count_mutex;
void reset_reading_user_count();
void increment_reading_user_count();
void decrement_reading_user_count();

extern int entry_array_size;
extern struct user_entry entry_array[CLIENT_MAX];
extern pthread_mutex_t entry_array_mutex;
void reset_entry_array();
void sort_entry_array();
/* https://www.tutorialspoint.com/c_standard_library/c_function_qsort.htm */
void output_entry_sorter();
void add_entry(struct user_entry entry);

/* endregion shared data */

/**
 * @brief 
 * 
 * @param buf 
 * @param mask
 * @remark Make sure that buf is empty
 * To play safe keep buf 200 chars 
 */
void translate_masks(char *buf, uint32_t mask);

void *observer_thread(void *arg);

void *user_thread(void *arg);

void do_server(notapp_args arg);

#endif /* _SERVER_H_ */
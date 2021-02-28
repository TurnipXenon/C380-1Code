#include "server_data.h"

static bool timer_expired = false;
static pthread_mutex_t timer_expired_mutex = PTHREAD_MUTEX_INITIALIZER;
void set_timer_expired(bool status) {
    pthread_mutex_lock(&timer_expired_mutex);
    timer_expired = status;
    pthread_mutex_unlock(&timer_expired_mutex);
}

bool get_timer_expired() {
    return timer_expired;
}


static enum server_state server_state = DONE;
static pthread_mutex_t server_state_mutex = PTHREAD_MUTEX_INITIALIZER;
void set_server_state(enum server_state state) {
    pthread_mutex_lock(&server_state_mutex);
    server_state = state;
    pthread_mutex_unlock(&server_state_mutex);
}

enum server_state get_server_state() {
    return server_state;
}


static int user_count = 0;
static pthread_mutex_t user_count_mutex = PTHREAD_MUTEX_INITIALIZER;
void increment_user_count() {
    pthread_mutex_lock(&user_count_mutex);
    ++user_count;
    pthread_mutex_unlock(&user_count_mutex);
}

void decrement_user_count() {
    pthread_mutex_lock(&user_count_mutex);
    if (user_count > 0) {
        --user_count;
    } else {
        printf("Warning: decrementing user count when empty");
    }
    pthread_mutex_unlock(&user_count_mutex);
}


static int reading_user_count = 0;
static pthread_mutex_t reading_user_count_mutex;
void reset_reading_user_count();
void increment_reading_user_count();
void decrement_reading_user_count();

static int entry_array_size;
static struct user_entry entry_array[CLIENT_MAX];
static pthread_mutex_t entry_array_mutex;
void reset_entry_array();
void sort_entry_array();
/* https://www.tutorialspoint.com/c_standard_library/c_function_qsort.htm */
void output_entry_sorter();
void add_entry(struct user_entry entry);
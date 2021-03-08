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

static int reader_done_count = 0;
static int reader_count = 0;
static pthread_mutex_t reading_user_count_mutex;
/**
 * @brief Get the reading user count object
 * 
 * @return int 
 * READERS can't really access until the server state is at reading no need for a mutex
 */
int get_reading_user_count() {
    return reader_count;
}

int are_readers_done() {
    return reader_count >= reader_count;
}

void reset_reader_done_count() {
    reader_done_count = 0;
}

void increment_reading_user_count();
void decrement_reading_user_count();

static int writer_count = 0;
static pthread_mutex_t writer_count_mutex;
static int entry_array_size;
static struct user_entry entry_array[CLIENT_MAX];
static pthread_mutex_t entry_array_mutex;

/**
 * @brief Get the entry writer count object
 * 
 * This function will only be used during SORTING state.
 * At that point, all writers will try to finish their jobs and just yield.
 * So, we know that the count will always be decrementing
 * when this is used. Does not really need a mutex.
 * 
 * @return int 
 */
int get_entry_writer_count() {
    // assert(get_server_state == SORTING);
    return writer_count;
}

void reset_entry_array() {
    entry_array_size = 0;
}

void sort_entry_array() {
    /* todo: sort array here */
    entry_array_size = 0;
}

/* https://www.tutorialspoint.com/c_standard_library/c_function_qsort.htm */
void output_entry_sorter();
void add_entry(struct user_entry entry);
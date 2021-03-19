/**
 * @file notapp_base.h
 * @author Allan Manuba
 * 
 * @brief Base file where shared data structures and functions are
 * 
 */

#ifndef _NOTAPP_BASE_H_
#define _NOTAPP_BASE_H_

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h> /* To get host ip */

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/inotify.h>
#include <pthread.h>

#include <setjmp.h>
#include <stdlib.h> // for malloc
#include <signal.h>

#define EVENT_STRUCT_SIZE (sizeof (struct inotify_event))

#define BUF_SIZE 1024

#define DISCONNECT_CODE -10

#define CLIENT_HEADER "TIME\t\t\t\tHOST\t\t\tMONITORED\t\tEVENT\n"

#define AUTOKILL_TIME 60

/**
 * @brief determines whether a message from an observer is a notification or disconnection
 */
enum msg_type {
    NOTIFICATION,
    DISCONNECTION_OBSERVER,
};

/**
 * @struct observer_msg
 * @brief message received by the server thread from observer
 */
struct observer_msg {
    enum msg_type type; /**< @brief Message type */
    struct timeval tv;  /**< @brief Time message was received */
    int wd;             /**< @brief Watch descriptor */
    uint32_t mask;      /**< @brief Mask describing event */
    uint32_t cookie;    /**< @brief Unique cookie associating related events (for rename(2)) */
    uint32_t len;       /**< @brief Size of name field */
};

/**
 * @brief Helps the server determine whether the new client is an observer
 * or a user
 */
enum msg_identity {
    INTRO_OBSERVER,
    INTRO_USER
};

/**
 * @brief Used in the parser to determine what role the user wants to run
 */
enum role {
    UNKNOWN,
    SERVER,
    OBSERVER_CLIENT,
    USER_CLIENT
};

/**
 * @brief Data structure returned after parsing through the commandline arguments
 */
typedef struct notapp_args {
    enum role role;
    float interval;
    char *sport;
    char *logfile;
    char *saddr;
    char *fileordir;
} notapp_args;

/**
 * @brief Stores context during sigsetjumps in user client and observer client
 */
sigjmp_buf env;

/**
 * @brief Handles segfault and sigjumps program to alert disconnect to server
 * 
 * @param sig 
 */
void handle_signal(int sig);

/**
 * @brief Checks whether the value sent was a DISCONNECT_CODE
 * 
 * @param val 
 * @return true Value sent was a DISCONNECT_CODE
 * @return false 
 */
bool is_disconnect(void* val);

/**
 * @brief Sends a string
 * 
 * @param sock 
 * @param string 
 */
void send_string(int sock, char *string);

/**
 * @brief Reads a string
 * 
 * @param sock 
 * @return char* string stored in heap, use free after use
 * @return NULL reading string failed
 */
char *read_string(int sock);

/**
 * @brief Clears screen
 */
void clear_screen();

/**
 * @brief Create a disconnect observer message object
 * 
 * @return struct observer_msg 
 */
struct observer_msg create_disconnect_observer_message();

/**
 * @brief Run on client to kill themselves when they are dormant for some time
 * 
 * @return void* 
 */
void *auto_kill();

/**
 * @brief Toggles is_dormant to false indicating the process is not dormant
 */
void alert_activity();

#endif /* _NOTAPP_BASE_H_ */
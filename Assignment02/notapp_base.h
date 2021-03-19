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

#include <setjmp.h>
#include <stdlib.h> // for malloc
#include <signal.h>

/* size of the event structure, not counting name */
#define EVENT_STRUCT_SIZE (sizeof (struct inotify_event))

/* usual buffer size */
#define BUF_SIZE 1024

#define DISCONNECT_CODE -10

#define CLIENT_HEADER "TIME\t\t\t\tHOST\t\t\tMONITORED\t\tEVENT\n"

enum msg_type {
    NOTIFICATION,
    DISCONNECTION_OBSERVER,
};

struct observer_msg {
    enum msg_type type;
    struct timeval tv;
    int wd;            /* Watch descriptor */
    uint32_t mask;     /* Mask describing event */
    uint32_t cookie;   /* Unique cookie associating related events (for rename(2)) */
    uint32_t len;      /* Size of name field */
    // struct inotify_event event;  document removal due to unknown behavior???
};

enum msg_identity {
    INTRO_OBSERVER,
    INTRO_USER
};

enum role {
    UNKNOWN,
    SERVER,
    OBSERVER_CLIENT,
    USER_CLIENT
};

typedef struct notapp_args {
    enum role role;
    float interval;
    char *sport;
    char *logfile;
    char *saddr;
    char *fileordir;
} notapp_args;

typedef struct user_msg {
    struct timeval tv;
    char host[17];
    char monitored[BUF_SIZE];
    char event_loc[BUF_SIZE];
    uint32_t mask;
} user_msg;

/**
 * @brief Stores context during sigsetjumps in user client and observer client
 */
sigjmp_buf env;

void handle_signal(int sig);

bool is_disconnect(void* val);

/* todo: transfer to different header */
void send_string(int sock, char *string);

char *read_string(int sock);

int notapp_connect(notapp_args arg, enum msg_identity id);

void clear_screen();

struct observer_msg create_disconnect_observer_message();

#endif /* _NOTAPP_BASE_H_ */
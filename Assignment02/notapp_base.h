#ifndef _NOTAPP_BASE_H_
#define _NOTAPP_BASE_H_

/* todo: clean up */
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

// region todo DELETE

/* size of the event structure, not counting name */
#define EVENT_STRUCT_SIZE (sizeof (struct inotify_event))

/* reasonable guess as to size of 1024 events */
#define BUF_LEN (1024 * (EVENT_STRUCT_SIZE + 16)) // todo: delete
// endregion todo DELETE

#define BUF_SIZE 1024

#define USER_SERVE_OUTPUT 1

#define DISCONNECT_CODE -10

enum msg_type {
    CONNECTION_OBSERVER,
    CONNECTION_USER,
    NOTIFICATION,
    DISCONNECTION_OBSERVER,
    DISCONNECTION_USER,
    SIZE_NOTIF,
    DYNAMIC_NOTIF
};

typedef struct notapp_msg {
    enum msg_type type;
    struct timeval tv;
    struct inotify_event event;
} notapp_msg;

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

#endif /* _NOTAPP_BASE_H_ */
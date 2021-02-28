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

#include "notapp_base.h"
#include "socket_helper.h"

typedef struct thread_arg {
    int sock;
    int interval;
} thread_arg;

#define THREAD_ARG_SIZE (sizeof (thread_arg))

void observer_thread(void *arg);

void user_thread(void *arg);

void do_server(notapp_args arg);

#endif /* _SERVER_H_ */
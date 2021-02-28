#ifndef _SERVER_H_
#define _SERVER_H_

#include <sys/time.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h> /* To get host ip */
#include <sys/inotify.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#include "notapp_base.h"
#include "socket_helper.h"
#include "server_data.h"

typedef struct thread_arg {
    int sock;
    int interval;
} thread_arg;

#define THREAD_ARG_SIZE (sizeof (thread_arg))

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
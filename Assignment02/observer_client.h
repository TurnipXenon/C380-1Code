#ifndef _OBSERVER_CLIENT_H_
#define _OBSERVER_CLIENT_H_

#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h> /* To get host ip */
#include <netdb.h>

#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#include "notapp_base.h"
#include "notapp_base.h"

#define EVENT_BUFFER_SIZE (10 * (EVENT_STRUCT_SIZE + NAME_MAX + 1))

/**
 * @brief Wait for inotify events and send them to server
 * 
 * @param arg 
 */
void do_observer_client(notapp_args arg);

#endif /* _OBSERVER_CLIENT_H_ */
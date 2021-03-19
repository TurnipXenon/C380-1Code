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
#include <pthread.h>
#include <sched.h>

#include "notapp_base.h"
#include "server_data.h"

#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <fcntl.h>

/**
 * @brief Spawn threads for clients
 * 
 * @param arg 
 */
void do_server(notapp_args arg);

#endif /* _SERVER_H_ */
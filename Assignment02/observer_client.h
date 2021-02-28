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

#include "notapp_base.h"
#include "notapp_base.h"

/* todo: clean up lol */
void do_observer_client(notapp_args arg);

#endif /* _OBSERVER_CLIENT_H_ */
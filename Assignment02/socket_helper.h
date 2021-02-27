#ifndef _SOCKET_HELPER_H_
#define _SOCKET_HELPER_H_

/* todo: clean up */
#include <sys/socket.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define BUF_SIZE 1024

bool is_disconnect(void* val);

/* todo: transfer to different header */
void send_string(int sock, char *string);

char *read_string(int sock);

#endif /* _SOCKET_HELPER_H_ */
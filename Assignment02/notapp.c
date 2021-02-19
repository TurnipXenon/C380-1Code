/**
 * @file notapp.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-02-18
 * 
 * @copyright Copyright (c) 2021
 * 
 */

// todo: replace poll with epoll
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <string.h>

#include "argparser.h"

// region todo DELETE
/* size of the event structure, not counting name */
#define EVENT_SIZE (sizeof (struct inotify_event))

/* reasonable guess as to size of 1024 events */
#define BUF_LEN (1024 * (EVENT_SIZE + 16))
// endregion todo DELETE

static void handle_events(int file_desc, int *watch_desc) {
    // code based on 'man inotify'
    char buf[4096]
               __attribute__ ((aligned(__alignof__(struct inotify_event))));
    const struct inotify_event *event;
    ssize_t len;

    while(1) {
        // read events
        len = read(file_desc, buf, sizeof(buf));
        if (len == -1 && errno != EAGAIN) {
            perror("read");
            // todo: disconnect + clean up
            return;
        }

        /* From man notify:
        If the nonblocking read() found no events to read, then
        it returns -1 with errno set to EAGAIN. In that case,
        we exit the loop. */
        if (len <= 0) {
            break;
        }

        for (char *ptr = buf; ptr < buf + len;
                ptr += EVENT_SIZE + event->len) {
            
            event = (const struct inotify_event *) ptr;

            // this is where we send our stuff out

            /* Identify event type here */
            if (event->mask & IN_OPEN)
                printf("IN_OPEN: ");
            if (event->mask & IN_CLOSE_NOWRITE)
                printf("IN_CLOSE_NOWRITE: ");
            if (event->mask & IN_CLOSE_WRITE)
                printf("IN_CLOSE_WRITE: ");

            /* Print the name of the watched directory */

            /* Print the name of the file */
            if (event->len)
                printf("%s", event->name);

            /* Print type of filesystem object */
            if (event->mask & IN_ISDIR)
                printf(" [directory]\n");
            else
                printf(" [file]\n");
        }
    }
}

static void do_observer_client(notapp_args arg) {
    // region Set up socket
    /* Code based on https://www.geeksforgeeks.org/socket-programming-cc/ */
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};  

    printf("Creating socket\n");
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(arg.sport);

    
    printf("Converting to binary\n");
    // Convert address to binary form
    if (inet_pton(AF_INET, arg.saddr, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        return;
    }

    printf("Connecting..\n");
    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr))) {
        perror("Connection failed");
        return;
    }
    
    printf("Sending message\n");
    send(sock , hello , strlen(hello) , 0); 
    printf("Hello message sent\n");
    // valread = read( sock , buffer, 1024); 
    // printf("%s\n",buffer ); 

    return;
    // endregion

    // todo attach a sigint handle here
    // code base 'man inotify'
    char buf;
    int file_desc, poll_num;
    int watch_desc;
    nfds_t nfds = 1;
    struct pollfd fds[1];

    // Initialize inotify
    file_desc = inotify_init();
    if (file_desc == -1) {
        perror("inotify_init");
        // todo: notify disconnection + clean up
        return;
    }

    // Add watcher
    watch_desc = inotify_add_watch(
        file_desc,
        arg.fileordir,
        IN_ALL_EVENTS
    );

    // Inotify input
    fds[0].fd = file_desc;
    fds[0].events = POLLIN; // todo may need to change???

    // Listen to events
    while(1) {
        poll_num = poll(fds, nfds, -1);
        if (poll_num == -1) {
            if (errno = EINTR) {
                // reissue system call
                continue;
            }

            /* else */
            perror(poll);
            // todo: notify disconnection + clean up
            return;
        }

        if (poll_num > 0) {
            if (fds[0].revents & POLLIN) {
                handle_events(file_desc, watch_desc);
            }
        }

        break;
    }

    printf("Event size: %d\n", EVENT_SIZE);
    printf("Buf len: %d\n", BUF_LEN);

    printf("Success!\n");

    // todo: how to clean up after sigint???
    close(file_desc);
}

int main(int argc, char *argv[]) {
    notapp_args arg = parse_args(argc, argv);

    switch (arg.role) {
        case UNKNOWN:
            printf("todo: notapp error\n");
            break;
        case SERVER:
            printf("todo: do server\n");
            break;
        case OBSERVER_CLIENT:
            do_observer_client(arg);
            break;
        case USER_CLIENT:
            printf("todo: user client\n");
            break;
        default:
            printf("todo: Not handled\n");
            break;
    }
}
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

// send said struct
// todo: replace poll with epoll
#include <sys/time.h>
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
#define BUF_SIZE 500

/* size of the event structure, not counting name */
#define EVENT_SIZE (sizeof (struct inotify_event))

/* reasonable guess as to size of 1024 events */
#define BUF_LEN (1024 * (EVENT_SIZE + 16))
// endregion todo DELETE

enum msg_type {
    CONNECTION_OBSERVER,
    CONNECTION_USER,
    NOTIFICATION,
    DISCONNECTION_OBSERVER,
    DISCONNECTION_USER
};

typedef struct notapp_msg {
    enum msg_type type;
    struct inotify_event event;
    struct timeval tv;
} notapp_msg;

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

void do_user_client(notapp_args arg) {
    // todo: listen to server

    // todo: fail gracefully lol

    // todo: clear terminal screen
}

/* region server */

void observer_thread() {
    /* it collects, as quickly as possible, data received from the observerclients and 
    updates a common data structure which stores the most recent event that came from 
    each observer. If a logfile was specified, the server also dumps the received events 
    to the log file, in the order they are received. */
}

void user_thread() {
    /* The threads serving the user clients are periodically (with <interval> period) 
    sending the updated information of this common data structure to their 
    corresponding user clients, so that it can be rendered. It is expected that if
    there are M user clients connected at the moment the periodic update is triggered, 
    all M clients receiveconsistent, i.e., identical, information. In other words, 
    the threads serving the user clients "broadcast" the currentstate of most recent 
    events to all simultaneously connected user clients. */
}

void do_server(notapp_args arg) {


    // based on https://www.geeksforgeeks.org/socket-programming-cc/
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0}; 
    char *hello = "Hello from server"; 

    // todo: bind to port
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed");
        return;
    }

    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 

    int sport = atoi(arg.sport);
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( sport ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 

    // if (listen(server_fd, 3) < 0) 
    // { 
    //     perror("listen"); 
    //     exit(EXIT_FAILURE); 
    // } 

    // if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
    // { 
    //     perror("accept"); 
    //     exit(EXIT_FAILURE); 
    // } 

    // if (sport == 0) {
    //     // from: https://stackoverflow.com/a/4047837/10024566
    //     socklen_t len = sizeof(address);
    //     if (getsockname(server_fd, (struct sockaddr *)&address, &len) == -1)
    //         perror("getsockname");
    //     else
    //         printf("port number %d\n", ntohs(address.sin_port));
    // }

    printf("Success\n");

    // todo: become daemon
    int i = fork();

    if (i < 0) exit(1); /* fork error */
    if (i > 0) exit(0); /* kill parent */

    while(1) {
        /* evil thoughts */
    }

    /* daemon time >:)c */

    // wait for clients and keep track of them
}

/* endregion server */

int main(int argc, char *argv[]) {
    notapp_args arg = parse_args(argc, argv);

    switch (arg.role) {
        case UNKNOWN:
            printf("todo: notapp error\n");
            break;
        case SERVER:
            do_server(arg);
            break;
        case OBSERVER_CLIENT:
            do_observer_client(arg);
            break;
        case USER_CLIENT:
            do_user_client(arg);
            break;
        default:
            printf("todo: Not handled\n");
            break;
    }
}
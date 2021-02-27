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

/**
 * Notes to self:
 * @remark 21 02 26 Fri
 * I have been focusing too much on the fact that servers
 * and clients share a space. There might be a case where
 * the server and client are not on the same machine. So,
 * I have to be very careful.
 * 
 * Most of my errors are from whether I should pass a pointer or not. Aaaaaah!
 * So always pass a pointer in send and read...
 * 
 * Rule: if we can cast it to -1, then it's a failure or disconnect
 */

// send said struct
// todo: replace poll with epoll
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
#include <pthread.h>
#include <stdbool.h>

#include "argparser.h"
#include "socket_helper.h"

// region todo DELETE

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
    DISCONNECTION_USER,
    SIZE_NOTIF,
    DYNAMIC_NOTIF
};

typedef struct notapp_msg {
    enum msg_type type;
    struct inotify_event event;
    struct timeval tv;
} notapp_msg;

enum msg_identity {
    INTRO_OBSERVER,
    INTRO_USER
};


typedef struct observer_init {
    char* host;
    char* monitored;
} observer_init;

typedef struct dynamic_msg {
    enum msg_type type;
    size_t size;
    void *body;
} dynamic_msg;

#define SIZE_MSG_SIZE (sizeof(dynamic_msg) + sizeof(size_t))

size_t get_dynamic_msg_size(size_t msg_size) {
    return msg_size + sizeof(dynamic_msg); // body + base
}

/* todo: handle error */
dynamic_msg *read_dynamic_file(int sock) {
    /* Read the body size first */
    size_t body_size;
    int valread1 = read(sock, &body_size, sizeof(size_t));
    printf("Size of body read is %zu\n", body_size);
    size_t overall_size = get_dynamic_msg_size(body_size);
    dynamic_msg *msg = malloc(overall_size);
    msg->size = body_size;
    printf("Base size: %zu\n", sizeof(dynamic_msg));
    printf("Dybamic size: %zu\n", overall_size);
    int valread2 = read(sock, msg, overall_size);
    char *uwu = (char*) msg->body;
    printf("Received size: %zu\n", msg->size);
    printf("Received: %c\n", uwu[0]);
    return msg;
}

/* todo: handle error */
void send_dynamic_file(int sock, void *body, size_t size) {
    /* Send the body size first */
    printf("Size of body sent is %zu\n", size);
    send(sock, &size, sizeof(size_t), 0);

    /* Send the message */
    size_t overall_size = get_dynamic_msg_size(size);
    dynamic_msg *msg = malloc(overall_size);
    msg->size = size;
    msg->body = &body;
    send(sock, msg, overall_size, 0);
    free(msg);
}

typedef struct thread_arg {
    int sock;
    int interval;
} thread_arg;

#define THREAD_ARG_SIZE (sizeof (thread_arg))

static void handle_events(int file_desc, int *watch_desc, int sock, char* monitored) {
    // code based on 'man inotify'
    char buf[4096]
               __attribute__ ((aligned(__alignof__(struct inotify_event))));
    struct inotify_event *event;
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

            struct notapp_msg event_message;
            event_message.type = NOTIFICATION;
            event_message.event = *event;
            gettimeofday(&event_message.tv, NULL);

            send(sock, &event_message, sizeof(event_message) , 0); 

            // this is where we send our stuff out

            /* Identify event type here */
            // if (event->mask & IN_OPEN)
            //     printf("IN_OPEN: ");
            // if (event->mask & IN_CLOSE_NOWRITE)
            //     printf("IN_CLOSE_NOWRITE: ");
            // if (event->mask & IN_CLOSE_WRITE)
            //     printf("IN_CLOSE_WRITE: ");

            // /* Print the name of the watched directory */

            // /* Print the name of the file */
            // if (event->len)
            //     printf("%s", event->name);

            // /* Print type of filesystem object */
            // if (event->mask & IN_ISDIR)
            //     printf(" [directory]\n");
            // else
            //     printf(" [file]\n");
        }
    }
}


static void do_observer_client(notapp_args arg) {
    // region Set up socket
    /* Code based on https://www.geeksforgeeks.org/socket-programming-cc/ */
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    observer_init init;
    init.host = strdup("???");
    init.monitored = strdup(arg.fileordir);
    printf("host: %s\n", init.host);
    printf("host po: %p\n", &init.host);
    char* testing = "Tomato";
    char buffer[1024] = {0};  
    
    /* Set up sig int here */
    if (0) {
        free(init.host);
        free(init.monitored);
        exit(0);
    }

    printf("Creating socket\n");
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return;
    }

    int sport = atoi(arg.sport);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(sport);
    
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

    /* Send identity */
    enum msg_identity id = INTRO_OBSERVER;
    printf("Sending identity\n");
    send(sock , &id, sizeof(id), 0);
    printf("Finish sending id\n");


    /* Send monitored file size */
    send_string(sock, arg.fileordir);
    // send(sock, arg.fileordir, strlen(arg.fileordir), 0);

    /* Send host ip todo */
    /* Code from https://www.geeksforgeeks.org/c-program-display-hostname-ip-address/ */
    {
        char hostbuffer[256]; 
        char *IPbuffer; 
        struct hostent *host_entry; 
        int hostname; 
    
        // To retrieve hostname 
        hostname = gethostname(hostbuffer, sizeof(hostbuffer)); 
        // checkHostName(hostname); 
    
        // To retrieve host information 
        host_entry = gethostbyname(hostbuffer); 
        // checkHostEntry(host_entry); 
    
        // To convert an Internet network 
        // address into ASCII string 
        IPbuffer = inet_ntoa(*((struct in_addr*) 
                            host_entry->h_addr_list[0])); 
    
        // printf("Hostname: %s\n", hostbuffer); 
        printf("Host IP: %s\n", IPbuffer); 
        send_string(sock, IPbuffer);
        // send(sock, IPbuffer, strlen(IPbuffer), 0);
    }

    exit(0);

    {
        
        // size_t size = strlen(arg.fileordir) + 1;
        // printf("Sending monitored: %zu\n", size);
        // send(sock, &size, sizeof(size_t), 0);
        // printf("Sending file %s\n", arg.fileordir);
        // send(sock, arg.fileordir, size, 0);
    }

    // // here in do_observer

    // struct notapp_msg init_message;
    // init_message.type = CONNECTION_OBSERVER;
    // printf("Sending message\n");
    // send(sock , &init_message, sizeof(init_message) , 0); 

    /* establish connection */

    // printf("Hello message sent\n");
    // valread = read( sock , buffer, 1024); 
    // printf("%s\n",buffer ); 

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
                printf("f1 Sending monitored: %p\n", arg.fileordir);
                handle_events(file_desc, watch_desc, sock, arg.fileordir);
            }
        }
    }

    close(file_desc);
}

void do_user_client(notapp_args arg) {
    // todo: listen to server

    // todo: fail gracefully lol

    // todo: clear terminal screen
}

/* region server */

void observer_thread(void *arg) {
    /* it collects, as quickly as possible, data received from the observerclients and 
    updates a common data structure which stores the most recent event that came from 
    each observer. If a logfile was specified, the server also dumps the received events 
    to the log file, in the order they are received. */
    char *monitored = NULL;
    char *ip_host = NULL;
    thread_arg *t_arg = (thread_arg*)arg;

    // {
    //     printf("Staring thread\n");
    //     int valread = read(t_arg->sock, monitored, BUF_SIZE);
    //     if (valread == -1) {
    //         printf("Oh no!\n");
    //         return;
    //     }
    //     if (is_disconnect(monitored)) {
    //         /* todo: clean up??? */
    //         return;
    //     }
    //     printf("Read monitored\n");
    // }

    // {
    //     int valread = read(t_arg->sock, ip_buffer, BUF_SIZE);
    //     if (valread == -1) {
    //         printf("Oh no!\n");
    //         return;
    //     }
    //     if (is_disconnect(ip_buffer)) {
    //         /* todo: clean up??? */
    //         return;
    //     }
        
    //     printf("Read monitored\n");
    // }

    monitored = read_string(t_arg->sock);
    ip_host = read_string(t_arg->sock);

    printf("Monitored stuff: %s\n", monitored);
    printf("From stuff: %s\n", ip_host);

    free(ip_host);
    free(monitored);

    return;

    /* Get init information from observer */
    /* Get monitored file size then name */
    // printf("Monitor: %s\n", (char*)msg_monitor->body);
    // free(msg_monitor);
    // {
    //     // size_t size;
    //     // int valread = read(t_arg->sock, &size, sizeof(size_t));
    //     // monitored = malloc(size);
    //     // valread = read(t_arg->sock, monitored, size);
    // }

    /* In case of break! */
    // if (0) {
        
    // }

    return;

    while(1) {
        struct notapp_msg notification;
        int valread = read(t_arg->sock, &notification, sizeof(notification)); 
        struct inotify_event event = notification.event;

        // printf("Monitored: %s\n", notification.monitored);

        /* Identify event type here */
        if (event.mask & IN_OPEN)
            printf("IN_OPEN: ");
        if (event.mask & IN_CLOSE_NOWRITE)
            printf("IN_CLOSE_NOWRITE: ");
        if (event.mask & IN_CLOSE_WRITE)
            printf("IN_CLOSE_WRITE: ");

        /* Print the name of the watched directory */

        /* Print the name of the file */
        if (event.len)
            printf("%s", event.name);

        /* Print type of filesystem object */
        if (event.mask & IN_ISDIR)
            printf(" [directory]\n");
        else
            printf(" [file]\n");
    }

    /* todo: receive message here! */
}

void user_thread(void *arg) {
    /* The threads serving the user clients are periodically (with <interval> period) 
    sending the updated information of this common data structure to their 
    corresponding user clients, so that it can be rendered. It is expected that if
    there are M user clients connected at the moment the periodic update is triggered, 
    all M clients receiveconsistent, i.e., identical, information. In other words, 
    the threads serving the user clients "broadcast" the currentstate of most recent 
    events to all simultaneously connected user clients. */
    printf("User client time!\n");
}

pthread_t create_thread(int new_socket) {

    // printf("%s\n", buffer); 
    // send(new_socket , hello , strlen(hello) , 0 ); 
    // printf("Hello message sent\n"); 

    /* todo: determine what kind of thread here */
}

void do_server(notapp_args arg) {
    // sock stream time

    // based on https://www.geeksforgeeks.org/socket-programming-cc/
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0}; 
    char *hello = "Hello from server"; 
    int is_parent = 1; /* or not yet daemonized */

    // todo: bind to port
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed");
        return;
    }

    // Forcefully attaching socket to the port 8080 
    // if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
    //                                               &opt, sizeof(opt))) 
    // { 
    //     perror("setsockopt"); 
    //     exit(EXIT_FAILURE); 
    // } 

    int sport = atoi(arg.sport);
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(sport); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 

    while(1) {
        if (listen(server_fd, 3) < 0) 
        { 
            perror("listen"); 
            exit(EXIT_FAILURE); 
        } 

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
        { 
            perror("accept"); 
            exit(EXIT_FAILURE); 
        } 

        /* todo: create thread for new connection */
        enum msg_identity client_id;
        int valread = read(new_socket, &client_id, sizeof(client_id)); 
        pthread_t thread;
        thread_arg t_arg;
        t_arg.sock = new_socket;

        switch(client_id) {
            case INTRO_OBSERVER:
                printf("Creating observer thread\n");
                pthread_create(&thread, NULL, observer_thread, &t_arg);
                printf("Created observer thread\n");
                pthread_join(thread, NULL);
                printf("Thread joined\n");
                break;
            case INTRO_USER:
                pthread_create(&thread, NULL, user_thread, NULL);
                pthread_join(thread, NULL);
                break;
            default:
                printf("Unknown T.T\n");
                break;
        }

        if (is_parent) {
            /* Print port */
            if (sport == 0) {
                // from: https://stackoverflow.com/a/4047837/10024566
                socklen_t len = sizeof(address);
                if (getsockname(server_fd, (struct sockaddr *)&address, &len) == -1)
                    perror("getsockname");
                else
                    printf("port number %d\n", ntohs(address.sin_port));
            }

            /* todo: daemonize here */

            is_parent = 0;
        }

        printf("Connection success\n");
    }

    // todo: become daemon
    // int i = fork();

    // if (i < 0) exit(1); /* fork error */
    // if (i > 0) exit(0); /* kill parent */

    /* daemon time >:)c */

    // while(1) {

    // }

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
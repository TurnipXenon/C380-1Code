#include "server.h"

/**
 * @brief 
 * 
 * @param buf 
 * @param mask
 * @remark Make sure that buf is empty
 * To play safe keep buf 200 chars 
 */
void translate_masks(char *buf, uint32_t mask) {
    if (mask & IN_ACCESS) {
        strcat(buf, " IN_ACCESS");
    }
    if (mask & IN_ATTRIB) {
        strcat(buf, " IN_ATTRIB");
    }
    if (mask & IN_CLOSE_WRITE) {
        strcat(buf, " IN_CLOSE_WRITE");
    }
    if (mask & IN_CLOSE_NOWRITE) {
        strcat(buf, " IN_CLOSE_NO_WRITE");
    }
    if (mask & IN_CREATE) {
        strcat(buf, " IN_CREATE");
    }
    if (mask & IN_DELETE) {
        strcat(buf, " IN_DELETE");
    }
    if (mask & IN_DELETE_SELF) {
        strcat(buf, " IN_DELETE_SELF");
    }
    if (mask & IN_MODIFY) {
        strcat(buf, " IN_MODIFY");
    }
    if (mask & IN_MOVE_SELF) {
        strcat(buf, " IN_MOVE_SELF");
    }
    if (mask & IN_MOVED_FROM) {
        strcat(buf, " IN_MOVED_FROM");
    }
    if (mask & IN_MOVED_TO) {
        strcat(buf, " IN_MOVED_TO");
    }
    if (mask & IN_OPEN) {
        strcat(buf, " IN_OPEN");
    }
}

void *observer_thread(void *arg) {
    /* it collects, as quickly as possible, data received from the observerclients and 
    updates a common data structure which stores the most recent event that came from 
    each observer. If a logfile was specified, the server also dumps the received events 
    to the log file, in the order they are received. */
    char *monitored = NULL;
    char *ip_host = NULL;
    thread_arg *t_arg = (thread_arg*)arg;
    user_msg msg; /* to be sent to user client */

    monitored = read_string(t_arg->sock);
    strcpy(msg.monitored, monitored);
    free(monitored);

    ip_host = read_string(t_arg->sock);
    strcpy(msg.host, ip_host);
    free(ip_host);

    while(1) {
        struct notapp_msg notification;
        /* todo: notification size may not be enough */
        int valread = read(t_arg->sock, &notification, sizeof(notification)); 

        if (is_disconnect(&notification) || valread == 0) {
            break;
        }

        struct inotify_event event = notification.event;

        if (event.len) {
            valread = read(t_arg->sock, msg.event_loc, event.len + 1);
            printf("Received: %s\n", msg.event_loc);
        } else {
            msg.event_loc[0] = '\0';
        }

        /* Process events to formattable thing for user client */
        msg.tv = notification.tv;
        msg.mask = event.mask;

        /* Identify event type here */
        // if (event.mask & IN_OPEN)
        //     printf("IN_OPEN: ");
        // if (event.mask & IN_CLOSE_NOWRITE)
        //     printf("IN_CLOSE_NOWRITE: ");
        // if (event.mask & IN_CLOSE_WRITE)
        //     printf("IN_CLOSE_WRITE: ");

        /* Print the name of the watched directory */

        /* Print the name of the file */
        if (event.len) {
            strcpy(msg.event_loc, msg.event_loc);
        } else {
            msg.event_loc[0] = '\0';
        }

        printf("Testing read\n");
        char msg_buf[BUF_SIZE] = {0};
        char tmp_buf[BUF_SIZE] = {0};
        snprintf(msg_buf, BUF_SIZE - 1, "%ld.%06ld\t", msg.tv.tv_sec, msg.tv.tv_usec);
        snprintf(tmp_buf, BUF_SIZE - 1, "%s\t", msg.host);
        printf("Host: %s\n", msg.host);
        strcat(msg_buf, tmp_buf);
        snprintf(tmp_buf, BUF_SIZE - 1, "%s\t", msg.monitored);
        strcat(msg_buf, tmp_buf);

        if (event.len) {
            strcat(msg_buf, msg.event_loc);
        }

        translate_masks(msg_buf, msg.mask);
        
        printf("%s\n", msg_buf);

        // mask_buffer();
        /* todo: print masks here */

        printf("\n");
        printf("==============\n");
    }

    /* todo: gracefully disconnecting! */
    printf("Client disconnected!\n");

    /* https://stackoverflow.com/a/36568809/10024566 */
    static const long ok_return = 1;
    return (void*)&ok_return;
}

void *user_thread(void *arg) {
    /* The threads serving the user clients are periodically (with <interval> period) 
    sending the updated information of this common data structure to their 
    corresponding user clients, so that it can be rendered. It is expected that if
    there are M user clients connected at the moment the periodic update is triggered, 
    all M clients receiveconsistent, i.e., identical, information. In other words, 
    the threads serving the user clients "broadcast" the currentstate of most recent 
    events to all simultaneously connected user clients. */
    thread_arg *t_arg = (thread_arg*)arg;

    while(1) {
        break;
    }

    /* todo: gracefully disconnecting! */
    printf("Client disconnected!\n");

    /* https://stackoverflow.com/a/36568809/10024566 */
    static const long ok_return = 1;
    return (void*)&ok_return;
}

static void *output_sorter(void *arg) {
    while(1) {
        pthread_yield();

        /* I'm not modifying the timer so it's okay to read it */
        if (get_timer_expired()) {
            /* Now, I need to access it */
            set_timer_expired(false);
            printf("Timer reset\n");

            /* the server state must be DONE to transition to SORTING */
            while (get_server_state() != DONE) {
                pthread_yield();
            }

            printf("Flipping state\n");
            set_server_state(SORTING);

            /* todo: wait for all observers to finish writing */
            /* make sure that the writer count is 0 */
            while (get_entry_writer_count() != 0) {
                pthread_yield();
            }

            /* todo: now we are sorting */
            sort_entry_array(); /* might need the reader count here??? */
            printf("Sorting\n");


            // cut off for readers to join
            set_server_state(READING);

            /* watch for readers */
            while (!are_readers_done()) {
                pthread_yield();
            }

            // reset reader done count
            reset_reader_done_count();

            set_server_state(DONE);
            /* At this point, readers can now proceed to waiting back to sorting */
            printf("Done: let observers observe\n");
        }
    }
}

static void *server_timer(void *arg) {
    /* todo: set up timer */
    struct timespec delay = {5, 0};
    while(1) {
        nanosleep(&delay, NULL);
        /* flip switch being watched by threads */
        printf("Timer expired\n");
        set_timer_expired(true);
    }
}

void do_server(notapp_args arg) {
    // sock stream time

    // based on https://www.geeksforgeeks.org/socket-programming-cc/
    int jump_val;
    int server_fd, new_socket; 
    struct sockaddr_in address; 
    int addrlen = sizeof(address); 
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
       
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }

    (void)signal(SIGINT, handle_signal);
    jump_val = sigsetjmp(env, 1);

    /* Clean up */
    if (jump_val != 0) {
        close(server_fd);
        exit(0);
    }

    /* Start timer */
    {
        pthread_t thread;
        pthread_create(&thread, NULL, server_timer, NULL);
        pthread_detach(thread);
    }

    /* Start sorter */
    {
        pthread_t thread;
        pthread_create(&thread, NULL, output_sorter, NULL);
        pthread_detach(thread);
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
        if (valread == 0) {
            continue;
        }
        
        pthread_t thread;
        thread_arg t_arg;
        t_arg.sock = new_socket;

        switch(client_id) {
            case INTRO_OBSERVER:
                pthread_create(&thread, NULL, observer_thread, &t_arg);
                pthread_detach(thread);
                break;
            case INTRO_USER:
                pthread_create(&thread, NULL, user_thread, NULL);
                pthread_detach(thread);
                break;
            default:
                perror("Unknown client connected\n");
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
            // int i = fork();

            // if (i < 0) exit(1); /* fork error */
            // if (i > 0) exit(0); /* kill parent */

            /* daemon time >:)c */

            is_parent = 0;
        }

        printf("Connection success\n");
    }
}
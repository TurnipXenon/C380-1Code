#include "server.h"

/**
 * @brief Translates the mask into text
 * 
 * @param buf 
 * @param mask
 */
static void translate_masks(char *buf, uint32_t mask) {
    if (mask & IN_ACCESS) {
        strcat(buf, "IN_ACCESS ");
    }
    if (mask & IN_ATTRIB) {
        strcat(buf, "IN_ATTRIB ");
    }
    if (mask & IN_CLOSE_WRITE) {
        strcat(buf, "IN_CLOSE_WRITE ");
    }
    if (mask & IN_CLOSE_NOWRITE) {
        strcat(buf, "IN_CLOSE_NO_WRITE ");
    }
    if (mask & IN_CREATE) {
        strcat(buf, "IN_CREATE ");
    }
    if (mask & IN_DELETE) {
        strcat(buf, "IN_DELETE ");
    }
    if (mask & IN_DELETE_SELF) {
        strcat(buf, "IN_DELETE_SELF ");
    }
    if (mask & IN_MODIFY) {
        strcat(buf, "IN_MODIFY ");
    }
    if (mask & IN_MOVE_SELF) {
        strcat(buf, "IN_MOVE_SELF ");
    }
    if (mask & IN_MOVED_FROM) {
        strcat(buf, "IN_MOVED_FROM ");
    }
    if (mask & IN_MOVED_TO) {
        strcat(buf, "IN_MOVED_TO ");
    }
    if (mask & IN_OPEN) {
        strcat(buf, "IN_OPEN ");
    }
    if (mask & IN_IGNORED) {
        strcat(buf, "IN_IGNORED ");
    }
    if (mask & IN_ISDIR) {
        strcat(buf, "IN_ISDIR ");
    }
    if (mask & IN_Q_OVERFLOW) {
        strcat(buf, "IN_Q_OVERFLOW ");
    }
    if (mask & IN_UNMOUNT) {
        strcat(buf, "IN_UNMOUNT ");
    }
}

/**
 * @brief it collects, as quickly as possible, data received from the observer
 * clients and updates a common data structure which stores the most recent
 * event that came from each observer. If a logfile was specified, the server
 * also dumps the received events to the log file, in the order they are received.
 * 
 * See also output_sorter.
 * 
 * @param arg 
 * @return void* 
 */
static void *observer_thread(void *arg) {
    int sock = *((int*)arg);
    char event_loc[BUF_SIZE];
    char monitored[BUF_SIZE];
    char host[17];
    struct user_entry entry;
    struct timeval tv;
    uint32_t mask;
    int observer_index = -1;

    /* Temporary variables */
    char *tmp_monitored = NULL;
    char *ip_host = NULL;

    tmp_monitored = read_string(sock);
    strcpy(monitored, tmp_monitored);
    free(tmp_monitored);

    ip_host = read_string(sock);
    strcpy(host, ip_host);
    free(ip_host);

    /* Register self to server data */
    observer_index = register_observer();

    /* Clean up entry */
    entry.tv = (struct timeval){0,0};
    entry.string[0] = '\0';
    add_entry(&entry, observer_index);

    /* Receive indicies */
    while(observer_index != -1) {
        struct observer_msg notification;
        int valread = read(sock, &notification, sizeof(notification)); 

        if (notification.type == DISCONNECTION_OBSERVER || valread <= 0) {
            break;
        }
        
#ifdef NOTAPP_TIME
        gettimeofday(&tv, NULL);
#else
        tv = notification.tv;
#endif

        if (notification.len) {
            valread = read(sock, event_loc, notification.len + 1);

            if (is_disconnect(event_loc) || valread == 0) {
                break;
            }
        } else {
            event_loc[0] = '\0';
        }

        /* Process events to formattable thing for user client */

        mask = notification.mask;

        /* Print the name of the file */
        if (!notification.len) {
            event_loc[0] = '\0';
        }

        char msg_buf[BUF_SIZE] = {0};
        char tmp_buf[BUF_SIZE] = {0};
        snprintf(msg_buf, BUF_SIZE - 1, "%ld.%06ld\t", tv.tv_sec, tv.tv_usec);
        snprintf(tmp_buf, BUF_SIZE - 1, "%s\t", host);
        strcat(msg_buf, tmp_buf);
        snprintf(tmp_buf, BUF_SIZE - 1, "%s\t", monitored);
        strcat(msg_buf, tmp_buf);

        if (notification.len) {
            strcat(msg_buf, event_loc);
            strcat(msg_buf, " ");
        }

        translate_masks(msg_buf, mask);

        strcpy(entry.string, msg_buf);
        entry.tv = tv;
        
        /* write on entry array */
        add_entry(&entry, observer_index);
    }

    unregister_observer(observer_index);

    /* https://stackoverflow.com/a/36568809/10024566 */
    static const long ok_return = 1;
    return (void*)&ok_return;
}

/**
 * @brief Print activities
 * 
 * The threads serving the user clients are periodically (with <interval> period)
 * sending the updated information of this common data structure to their 
 * corresponding user clients, so that it can be rendered. It is expected that if
 * there are M user clients connected at the moment the periodic update is triggered, 
 * all M clients receiveconsistent, i.e., identical, information. In other words, 
 * the threads serving the user clients "broadcast" the currentstate of most recent 
 * events to all simultaneously connected user clients.
 * 
 * See also output_sorter
 * 
 * @param arg 
 * @return void* 
 */
static void *user_thread(void *arg) {
    int socket = *((int*)arg);
    int confirmation = 1;

    register_user();

    while(1) {
        while (get_server_state() != READING && !is_locked()) {
            sched_yield();
        }

        /* Is the user client dead? */
        int valread = read(socket, &confirmation, sizeof(int));
        if (is_disconnect(&confirmation) || valread == 0) {
            break;
        }

        send_entries(socket);
        
        while (get_server_state() == READING && !is_locked()) {
            sched_yield();
        }
    }

    unregister_user();

    /* https://stackoverflow.com/a/36568809/10024566 */
    static const long ok_return = 1;
    return (void*)&ok_return;
}

/**
 * @brief Serves as a traffic enforcer for all the threads
 * 
 * Output sorter has three phases, according to enum server_state: DONE, SORTING, and READING.
 * 
 * In DONE state:
 * - All observer threads are free to write.
 * - Output sorter is waiting.
 * - All user threads are waiting.
 * 
 * During the transition from DONE to SORTING:
 * - All observer threads are barred from writing since it's not SORTING state.
 * - Output sorter waits for all observer threads writing to get out.
 * - All user threads are waiting.
 * 
 * In SORTING state:
 * - All observer threads are waiting.
 * - Output sorter sorts.
 * - All user threads are waiting.
 * 
 * During the transition from SORTING to READING:
 * - All observer threads are waiting.
 * - Output sorter will simply change the server state since there's not much to do.
 * - All user threads are waiting.
 * 
 * In READING state:
 * - All observer threads are waiting.
 * - Output sorter writes on the log file if there's a log file.
 * - All user threads read and send the entries on one go. When done, they yield
 *   until a server state change.
 * 
 * During the transition from READING to DONE:
 * - All observer threads are waiting.
 * - Output sorter waits for all registered user clients to finish their job.
 * - When user threads are done, they increment a counter indicating so. The counter
 *   is used to ensure that all readers are done with their job, and output sorter
 *   can also find out if it can change the server state. Reset the reader counter after.
 * 
 * Tnere's also a mechanic that prevents deadlock due to concurrency and uncertainty from
 * socket communication. When the output sorter has not completed a full-rotation of all
 * the phases in under two intervals via server_timer, then we assume that we are stuck
 * in a lock. Timer triggers this is_locked(), and all threads reliant on the server state
 * will make a full sprint back to the server state DONE. Threads reliant on the server state
 * include the output_sorter and user_thread.
 * 
 * @param arg 
 * @return void* 
 */
static void *output_sorter(void *arg) {
    char* logfile = ((char*)arg);

    while(1) {
        sched_yield();

        if (get_timer_expired()) {
            /* Now, I need to access it */
            struct timeval tv;
            gettimeofday(&tv, NULL);
            set_timer_expired(false);

            /* The server state must be DONE to transition to SORTING */
            while (get_server_state() != DONE && !is_locked()) {
                sched_yield();
            }

            set_server_state(SORTING);

            /* make sure that the writer count is 0 */
            while (get_entry_writer_count() != 0 && !is_locked()) {
                sched_yield();
            }

            sort_entry_array();

            /* cut off for readers to join */
            set_server_state(READING);

            /* write on logfile  */
            if (logfile != NULL) {
                print_logfile(logfile);
            }

            /* watch for readers */
            while (!are_readers_done() && !is_locked()) {
                sched_yield();
            }

            reset_reader_done_count();

            set_server_state(DONE);
            /* At this point, readers can now proceed to waiting back to sorting */
        }
    }

    /* based on https://stackoverflow.com/a/36568809/10024566 */
    static const long ok_return = 1;
    return (void*)&ok_return;
}

/**
 * @brief Serves as a timer
 * 
 * It also helps release processes when they get stuck in a possible "locked" situation.
 * 
 * @param arg 
 * @return void* 
 */
static void *server_timer(void *arg) {
    const long nanosec_in_sec = 1000000000;
    float interval = *((float*)arg);
    struct timespec delay = {
        (int)interval,
        ((long)(interval * nanosec_in_sec) % nanosec_in_sec)
    };

    while(1) {
        nanosleep(&delay, NULL);
        /* flip switch being watched by threads */
        set_timer_expired(true);
    }

    /* https://stackoverflow.com/a/36568809/10024566 */
    static const long ok_return = 1;
    return (void*)&ok_return;
}

/**
 * @brief Daemonizes the program
 */
static void daemonize(int sock) {
    int i;
	if (getppid() == 1) return; /* already a daemon */
	i = fork();
	if (i < 0) exit(EXIT_FAILURE); /* fork error */
	if (i > 0) exit(EXIT_SUCCESS); /* parent exits */

	/* child (daemon) continues */
	setsid(); /* obtain a new process group */
    
    /* Second fork */
	i = fork();
	if (i < 0) exit(EXIT_FAILURE); /* fork error */
	if (i > 0) exit(EXIT_SUCCESS); /* parent exits */
    
    for (i=getdtablesize();i>=0;--i) {
        if (i == sock) {
            continue;
        }

        close(i); /* close all descriptors */
    }

    i = open("/dev/null", O_RDWR);
    dup(i); dup(i); /* handle standard I/O */

	umask(027); /* set newly created file permissions */
	
    /* first instance continues */
	signal(SIGCHLD,SIG_IGN); /* ignore child */
	signal(SIGTSTP,SIG_IGN); /* ignore tty signals */
	signal(SIGTTOU,SIG_IGN);
	signal(SIGTTIN,SIG_IGN);
}

/**
 * @brief Accepts connection and spawns threads
 * 
 * @param arg 
 */
void do_server(notapp_args arg) {
    // based on https://www.geeksforgeeks.org/socket-programming-cc/
    int jump_val;
    int server_fd, new_socket; 
    struct sockaddr_in address; 
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed");
        return;
    }

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
        
        openlog ("notapp", LOG_PID, LOG_DAEMON);
        syslog (LOG_NOTICE, "notapp daemon terminated.");
        closelog();

        exit(0);
    }

    socklen_t len = sizeof(address);
    if (sport == 0) {
        if (getsockname(server_fd, (struct sockaddr *)&address, &len) == -1)
            perror("getsockname");
        else
            printf("Port number %d\n", ntohs(address.sin_port));
    }

    daemonize(server_fd);

    /* Start timer */
    {
        pthread_t thread;
        pthread_create(&thread, NULL, server_timer, &arg.interval);
        pthread_detach(thread);
    }

    /* Start sorter */
    {
        pthread_t thread;
        pthread_create(&thread, NULL, output_sorter, arg.logfile);
        pthread_detach(thread);
    }

    /* Accept clients and spawn threads for each */
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

        enum msg_identity client_id;
        int valread = read(new_socket, &client_id, sizeof(client_id)); 
        if (valread == 0) {
            continue;
        }
        
        pthread_t thread;

        switch(client_id) {
            case INTRO_OBSERVER:
                pthread_create(&thread, NULL, observer_thread, &new_socket);
                pthread_detach(thread);
                break;
            case INTRO_USER:
                pthread_create(&thread, NULL, user_thread, &new_socket);
                pthread_detach(thread);
                break;
            default:
                perror("Unknown client connected\n");
                break;
        }
    }
}
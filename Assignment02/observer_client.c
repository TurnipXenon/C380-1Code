#include "observer_client.h"

/**
 * @brief Cleanup before finishing
 * 
 * @param sock 
 * @param file_desc 
 */
static cleanup(int sock, int file_desc) {
    struct observer_msg disconnect_msg = create_disconnect_observer_message();
    send(sock, &disconnect_msg, sizeof(disconnect_msg),0);
    close(sock);

    if (file_desc > 0) {
        close(file_desc);
    }
}

/**
 * @brief Wait for inotify events and send them to server
 * 
 * @param arg 
 */
void do_observer_client(notapp_args arg) {
    /* Set up socket
       Code based on https://www.geeksforgeeks.org/socket-programming-cc/ */
    int sock = 0;
    struct sockaddr_in serv_addr;
    int file_desc = -1;
    int jump_val;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return;
    }

    int sport = atoi(arg.sport);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(sport);
    
    if (inet_pton(AF_INET, arg.saddr, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        return;
    }

    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr))) {
        perror("Connection failed");
        return;
    }

    /* Notify server of kill signal */
    (void)signal(SIGINT, handle_signal);
    jump_val = sigsetjmp(env, 1);

    if (jump_val != 0) {
        cleanup(sock, file_desc);
        return;
    }

    /* Send identity */
    enum msg_identity id = INTRO_OBSERVER;
    send(sock , &id, sizeof(id), 0);

    /* Send monitored file size */
    send_string(sock, arg.fileordir);

    /* Send host ip todo
       Code from https://www.geeksforgeeks.org/c-program-display-hostname-ip-address/ */
    {
        char hostbuffer[256]; 
        char *IPbuffer; 
        struct hostent *host_entry; 
    
        gethostname(hostbuffer, sizeof(hostbuffer)); 
    
        host_entry = gethostbyname(hostbuffer); 
    
        /* Convert an Internet network address into ASCII string */
        IPbuffer = inet_ntoa(*((struct in_addr*) 
                            host_entry->h_addr_list[0])); 
    
        send_string(sock, IPbuffer);
    }


    /* Initialize inotify */
    file_desc = inotify_init();
    if (file_desc == -1) {
        cleanup(sock, file_desc);
        perror("inotify_init");
        return;
    }

    /* Add watcher */
    inotify_add_watch(
        file_desc,
        arg.fileordir,
        IN_ALL_EVENTS
    );

    /* Inotify input */
    char buffer[EVENT_BUFFER_SIZE];
    int i = 0;

    /* Set up auto kill */
    pthread_t thread;
    pthread_create(&thread, NULL, auto_kill, NULL);
    pthread_detach(thread);

    /* Listen to events and send */
    while(1) {
        alert_activity();

        int bytesRead = read(file_desc, buffer, BUF_SIZE), bytesProcessed = 0;
        if(bytesRead < 0) {
            continue;
        }

        while(bytesProcessed < bytesRead) {
            struct inotify_event* event = (struct inotify_event*)(buffer + bytesProcessed);
            
            struct observer_msg event_message;
            event_message.type = NOTIFICATION;
            gettimeofday(&event_message.tv, NULL);
            event_message.wd = event->wd;
            event_message.mask = event->mask;
            event_message.cookie = event->cookie;
            event_message.len = event->len;

            send(sock, &event_message, sizeof(event_message) , 0); 

            if (event->len) {
                send(sock, event->name, event->len + 1, 0);
            }

            bytesProcessed += EVENT_STRUCT_SIZE + event->len;
        }
    }
}
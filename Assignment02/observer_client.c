#include "observer_client.h"

/* todo: clean up lol */
void do_observer_client(notapp_args arg) {
    // region Set up socket
    /* Code based on https://www.geeksforgeeks.org/socket-programming-cc/ */
    int sock = 0;
    struct sockaddr_in serv_addr;
    
    // todo attach a sigint handle here
    // code base 'man inotify'
    int file_desc = -1;
    int jump_val;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return;
    }

    int sport = atoi(arg.sport);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(sport);
    
    // Convert address to binary form
    if (inet_pton(AF_INET, arg.saddr, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        return;
    }

    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr))) {
        perror("Connection failed");
        return;
    }

    (void)signal(SIGINT, handle_signal);
    jump_val = sigsetjmp(env, 1);

    /* Notify server about leaving */
    if (jump_val != 0) {
        int dummy_val = DISCONNECT_CODE;
        send(sock, &dummy_val, sizeof(dummy_val),0);
        close(sock);

        if (file_desc > 0) {
            close(file_desc);
        }

        printf("Sending closing\n");
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
    
        // To retrieve hostname 
        gethostname(hostbuffer, sizeof(hostbuffer)); 
        // checkHostName(hostname); 
    
        // To retrieve host information 
        host_entry = gethostbyname(hostbuffer); 
        // checkHostEntry(host_entry); 
    
        // To convert an Internet network 
        // address into ASCII string 
        IPbuffer = inet_ntoa(*((struct in_addr*) 
                            host_entry->h_addr_list[0])); 
    
        printf("Host IP: %s\n", IPbuffer); 
        send_string(sock, IPbuffer);
    }


    // Initialize inotify
    file_desc = inotify_init();
    if (file_desc == -1) {
        perror("inotify_init");
        // todo: notify disconnection + clean up
        return;
    }

    // Add watcher
    inotify_add_watch(
        file_desc,
        arg.fileordir,
        IN_ALL_EVENTS
    );

    // Inotify input
    char buffer[EVENT_BUFFER_SIZE];
    int i = 0;

    // Listen to events
    while(1) {
        // read 
        printf("[%d] Reading...\n", i);
        ++i;
        int bytesRead = read(file_desc, buffer, BUF_SIZE), bytesProcessed = 0;
        if(bytesRead < 0) // read error
            continue;

        printf("Sending...\n");
        while(bytesProcessed < bytesRead) {
            struct inotify_event* event = (struct inotify_event*)(buffer + bytesProcessed);
            
            struct notapp_msg event_message;
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
        printf("Finish sending...\n");
    }
}
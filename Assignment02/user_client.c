#include "user_client.h"

void do_user_client(notapp_args arg) {
    int success_code = 1;
    bool client_done = false;
    int sock = 0;
    int entry_count = 0;
    int index = 0;
    int valread = 0;
    struct sockaddr_in serv_addr;
    char *text = NULL;
    int jump_val;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(arg.sport));

    /* Connect to server
       Code based on https://www.geeksforgeeks.org/socket-programming-cc/ */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return;
    }
    
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
        int dummy_val = DISCONNECT_CODE;
        send(sock, &dummy_val, sizeof(dummy_val),0);
        close(sock);
        return;
    }

    /* Send identity */
    enum msg_identity id = INTRO_USER;
    send(sock , &id, sizeof(id), 0);

    /* Set up auto kill */
    pthread_t thread;
    pthread_create(&thread, NULL, auto_kill, NULL);
    pthread_detach(thread);

    /* Print content */
    clear_screen();
    while (1) {
        alert_activity();
        
        send(sock, &success_code, sizeof(success_code), 0);

        /* Wait for the count of entries to print */
        valread = read(sock, &entry_count, sizeof(int));
        if(valread == 0 || is_disconnect(&entry_count)) {
            break;
        }

        clear_screen();
        printf(CLIENT_HEADER);
        for (index = 0; index < entry_count; ++index) {
            text = read_string(sock);
            if (strlen(text) > 0) {
                printf("%s\n", text);
            }
            free(text);
        }

        if(client_done) {
            break;
        }
    }

    int dummy_val = DISCONNECT_CODE;
    send(sock, &dummy_val, sizeof(dummy_val),0);
    close(sock);
}

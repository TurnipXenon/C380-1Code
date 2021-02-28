#include "user_client.h"

void do_user_client(notapp_args arg) {
    bool client_done = false;
    int sock = 0;
    int entry_count = 0;
    int entry_size = 0;
    int index = 0;
    int valread = 0;
    char buf[BUF_SIZE];
    struct sockaddr_in serv_addr;
    
    // todo attach a sigint handle here
    // code base 'man inotify'
    int jump_val;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(arg.sport));

    /* Code based on https://www.geeksforgeeks.org/socket-programming-cc/ */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return;
    }
    
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
        int dummy_val = 1;
        send(sock, &dummy_val, sizeof(dummy_val),0);
        close(sock);
        return;
    }

    /* Send identity */
    enum msg_identity id = INTRO_USER;
    send(sock , &id, sizeof(id), 0);

    while (1) {
        printf("%s\t%s\t%s\t%s\n", "TIME", "HOST", "MONITORED", "EVENT");
break;

        /* Wait for the count of entries to print */
        valread = read(sock, &entry_count, sizeof(int));
        if(valread == 0 || is_disconnect(&entry_count)) {
            break;
        }

        printf("%s\t%s\t%s\t%s\n", "TIME", "HOST", "MONITORED", "EVENT");
        for (index = 0; index < entry_count; ++index) {
            /* Find out the size of the string */
            valread = read(sock, &entry_size, sizeof(int));
            if (valread == 0) {
                client_done = true;
                break;
            }

            if (entry_size > BUF_SIZE) {
                printf("Exceeding entry size: %d\n", entry_size);
                client_done = true;
                break;
            }

            /* Get and print the string */
            valread = read(sock, &buf, entry_size);
            if (valread == 0) {
                client_done = true;
                break;
            }
            printf("%s\n", buf);
        }

        if(client_done) {
            break;
        }
    }

    int dummy_val = 1;
    send(sock, &dummy_val, sizeof(dummy_val),0);
    close(sock);
}

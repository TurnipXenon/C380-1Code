#include "socket_helper.h"

bool is_disconnect(void* val) {
    if (*((int*)val) == -1) {
        return true;
    } else {
        return false;
    }
}

/* todo: transfer to different header */
void send_string(int sock, char *string) {
    size_t str_size = strlen(string) + 1;
    printf("Sending size: %zu\n", str_size);

    /* Send the size first */
    send(sock, &str_size, sizeof(size_t), 0);

    /* Send the string */
    send(sock, string, str_size, 0);
}

char *read_string(int sock) {
    char *str;
    size_t str_size;

    /* Read size */
    {
        int valread = read(sock, &str_size, sizeof(size_t));

        if (valread == -1 || is_disconnect(&str_size)) {
            return NULL;
        }
    }

    /* Read string */
    str = malloc(str_size);
    {
        printf("str_size: %zu\n", str_size);
        int valread = read(sock, str, str_size);
        
        if (valread == -1 || is_disconnect(str)) {
            return NULL;
        }
    }

    return str;
}
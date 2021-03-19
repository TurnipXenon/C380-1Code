#include "notapp_base.h"

void handle_signal(int sig) {
    if (sig == SIGINT) {
        (void) signal(SIGINT, SIG_IGN);
    }

    siglongjmp(env, 1);
}


bool is_disconnect(void* val) {
    if (*((int*)val) == DISCONNECT_CODE) {
        return true;
    } else {
        return false;
    }
}

/* todo: transfer to different header */
void send_string(int sock, char *string) {
    size_t str_size = strlen(string) + 1;

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
        int valread = read(sock, str, str_size);
        
        if (valread == -1 || is_disconnect(str)) {
            return NULL;
        }
    }

    return str;
}

// https://stackoverflow.com/a/18154608/10024566
void clear_screen() {
#ifdef _WIN32
    system("cls");
#elif defined(unix) || defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
    system("clear");
#endif
}

struct observer_msg create_disconnect_observer_message() {
    struct observer_msg msg;
    msg.type = DISCONNECTION_OBSERVER;
    return msg;
}
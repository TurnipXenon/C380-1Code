#include "notapp_base.h"

/**
 * @brief Handles segfault and sigjumps program to alert disconnect to server
 * 
 * @param sig 
 */
void handle_signal(int sig) {
    if (sig == SIGINT) {
        (void) signal(SIGINT, SIG_IGN);
    }

    siglongjmp(env, 1);
}

/**
 * @brief Checks whether the value sent was a DISCONNECT_CODE
 * 
 * @param val 
 * @return true Value sent was a DISCONNECT_CODE
 * @return false 
 */
bool is_disconnect(void* val) {
    if (*((int*)val) == DISCONNECT_CODE) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief Sends a string
 * 
 * @param sock 
 * @param string 
 */
void send_string(int sock, char *string) {
    size_t str_size = strlen(string) + 1;

    /* Send the size first */
    send(sock, &str_size, sizeof(size_t), 0);

    /* Send the string */
    send(sock, string, str_size, 0);
}

/**
 * @brief Reads a string
 * 
 * @param sock 
 * @return char* string stored in heap, use free after use
 * @return NULL reading string failed
 */
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

/**
 * @brief Calls appropriate screen clearing functions depending on OS
 * Code by JSQuareD from https://stackoverflow.com/a/18154608/10024566
 */
void clear_screen() {
#ifdef _WIN32
    system("cls");
#elif defined(unix) || defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
    system("clear");
#endif
}

/**
 * @brief Create a disconnect observer message object by setting
 * the type to DISCONNECTION_OBSERVER
 * 
 * @return struct observer_msg 
 */
struct observer_msg create_disconnect_observer_message() {
    struct observer_msg msg;
    msg.type = DISCONNECTION_OBSERVER;
    return msg;
}

static bool is_dormant = false;
static pthread_mutex_t is_dormant_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief Toggles is_dormant to false indicating the process is not dormant
 */
void alert_activity() {
    pthread_mutex_lock(&is_dormant_mutex);
    is_dormant = false;
    pthread_mutex_unlock(&is_dormant_mutex);
}

/**
 * @brief Run on client to kill themselves when they are dormant for some time
 * 
 * @return void* 
 */
void *auto_kill() {
    while(1) {
        sleep(AUTOKILL_TIME);
        
        pthread_mutex_lock(&is_dormant_mutex);
        if (is_dormant) {
            raise(SIGINT);
        }

        is_dormant = true;
        pthread_mutex_unlock(&is_dormant_mutex);
    }

    static const long ok_return = 1;
    return (void*)&ok_return;
}
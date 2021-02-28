#include "notapp_base.h"

void handle_signal(int sig) {
    if (sig == SIGINT) {
        (void) signal(SIGINT, SIG_IGN);
    }

    siglongjmp(env, 1);
}
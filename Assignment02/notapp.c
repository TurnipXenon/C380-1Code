#include <errno.h>
#include <sys/inotify.h>

#include "argparser.h"

// region todo DELETE
/* size of the event structure, not counting name */
#define EVENT_SIZE (sizeof (struct inotify_event))

/* reasonable guess as to size of 1024 events */
#define BUF_LEN (1024 * (EVENT_SIZE + 16))
// endregion todo DELETE

void do_observer_client(notapp_args arg) {
    int file_desc = inotify_init();

    if (file_desc < 0) {
        perror("inotify_init");
        return;
    }
    
    int watch_desc = inotify_add_watch(
        file_desc,
        arg.fileordir,
        IN_ALL_EVENTS
    );

    if (watch_desc < 0) {
        perror("inotify_add_watch");
        return;
    }

    char buf[BUF_LEN];
    int len, i = 0;

    len = read(file_desc, buf, BUF_LEN);

    if (len < 0) {
        if (errno == EINTR) {
            /* need to reissue system call */
            printf("Need to reissue system call\n");
        } else {
            perror("read");
        }
        return;
    } else if (!len) {
        printf("BUF_LEN too small?\n");
        return;
    }

    while (i < len) {
        struct inotify_event *event;

        event = (struct inotify_event *) &buf[i];

        printf(
            "wd=%d mask=%u cookie=%u len=%u\n",
            event->wd,
            event->mask,
            event->cookie,
            event->len
        );

        if (event->len) {
            printf("name=%s\n", event->name);
        }

        i += EVENT_SIZE + event->len;
    }

    printf("Success!\n");
}

int main(int argc, char *argv[]) {
    notapp_args arg = parse_args(argc, argv);

    switch (arg.role) {
        case UNKNOWN:
            printf("todo: notapp error\n");
            break;
        case SERVER:
            printf("todo: do server\n");
            break;
        case OBSERVER_CLIENT:
            do_observer_client(arg);
            break;
        case USER_CLIENT:
            printf("todo: user client\n");
            break;
        default:
            printf("todo: Not handled\n");
            break;
    }
}
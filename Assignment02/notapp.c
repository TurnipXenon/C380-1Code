/**
 * @file notapp.c
 * @author Allan Manuba
 * @brief Client / server application monitoring files or directories
 * @date 2021-02-18
 * 
 */

/**
 * Notes to self:
 * @remark 21 02 26 Fri
 * I have been focusing too much on the fact that servers
 * and clients share a space. There might be a case where
 * the server and client are not on the same machine. So,
 * I have to be very careful.
 * 
 * Most of my errors are from whether I should pass a pointer or not. Aaaaaah!
 * So always pass a pointer in send and read...
 * 
 * Rule: if we can cast it to -1, then it's a failure or disconnect
 */

#include "notapp_base.h"
#include "argparser.h"
#include "socket_helper.h"
#include "server.h"
#include "user_client.h"
#include "observer_client.h"

int main(int argc, char *argv[]) {
    notapp_args arg = parse_args(argc, argv);

    switch (arg.role) {
        case UNKNOWN:
            printf("todo: notapp error\n");
            break;
        case SERVER:
            do_server(arg);
            break;
        case OBSERVER_CLIENT:
            do_observer_client(arg);
            break;
        case USER_CLIENT:
            do_user_client(arg);
            break;
        default:
            printf("todo: Not handled\n");
            break;
    }
}
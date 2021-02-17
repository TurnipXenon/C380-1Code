/**
 * @file argumentparser.h
 * @author Allan Manuba (manuba@ualberta.ca)
 * @brief Identifies the role and arguments passed to notapp.
 * 
 * Specific roles:
 * server:
 *      notapp -s -t <interval> [-p <sport>] [-l <logfile>]
 * observer client:
 *      notapp -o <saddr> <sport> <fileordir>
 * user client:
 *      notapp -u <saddr> <sport>
 * 
 * Where:
 * - <interval> indicates how frequently the server broadcasts to the user clients the most 
 * recent events of its observer clients. The interval can take values from 0.1 to 10 seconds.
 * - <sport> is the server port and it is optional. If not provided, the system chooses a 
 * port for the server. This port (the chosen by the system) should be printed by the server 
 * to stdout before it becomes a daemon process.
 * - <logfile> is an optional log file where the server keeps track of all the messages it 
 * received and any of the actions that it took. The log file is meant mainly for debugging 
 * so it has to be informative to you and the teaching assistants.
 * 
 * todo: move this somewhere
 * When starting as a client of either type (observer or user), the client connects to the 
 * server listening on port <sport> of the host while <saddr> IP address. In the case of an
 * observer client, the <fileordir> indicates the file or directory to monitor for all
 * possible inotify events.
 * 
 * @date 2021-02-17
 */

#ifndef _ARGUMENT_PARSER_H_
#define _ARGUMENT_PARSER_H_

#include <stdio.h>

enum role {
    SERVER,
    OBSERVER_CLIENT,
    USER_CLIENT
};

struct notapp_arguments {
    enum role role;
    float interval;
    char *logfile;
};

#endif /* _ARGUMENT_PARSER_H_ */
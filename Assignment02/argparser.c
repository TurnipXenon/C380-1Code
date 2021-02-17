#include "argparser.h"

notapp_args init_notapp_args() {
    notapp_args ret = {
        UNKNOWN, // role
        5.0f, // interval
        "127.0.0.1", // sport
        "logfile.txt", // logfile
        "127.0.0.1", // saddr
        "randomfile.txt" // fileordir
    };
    return ret;
}

// todo: private
notapp_args parse_args_server(int argc, char *argv[]) {
    notapp_args ret = init_notapp_args();

// todo: put assumptions
    if (argc < 3) {
        // todo: error message here
        printf("Not enough args todo\n");
        return ret;
    }

    int index = 2;
    bool interval_flag = false;

    while (index < argc) {
        if (index + 1 == argc) {
            printf("todo: handle no args value\n");
            return ret;
        }

        if (strcmp(argv[index], "-t") == 0) {
            char *pf;
            ret.interval = strtof(argv[index + 1], &pf);
            interval_flag = true;
            // todo: what if pf != null ???
            // todo: error???
        } else if (strcmp(argv[index], "-p") == 0) {
            ret.sport = argv[index + 1];
        } else if (strcmp(argv[index], "-l") == 0) {
            ret.logfile = argv[index + 1];
        } else {
            printf("todo: handle unknown flag\n");
            return ret;
        }

        index += 2;
    }

    if (!interval_flag) {
        printf("todo: required flag not present\n");
        return ret;
    }

    ret.role = SERVER;

    return ret;
}

// todo: private
notapp_args parse_args_observer_client(int argc, char *argv[]) {
    notapp_args ret = init_notapp_args();

    if (argc != 5) {
        printf("Not enough args todo\n");
        return ret;
    }

    ret.role = SERVER;
    ret.saddr = argv[2];
    ret.sport = argv[3];
    ret.fileordir = argv[4];

    return ret;
}

// todo: private
notapp_args parse_args_user_client(int argc, char *argv[]) {
    // todo
    notapp_args ret = init_notapp_args();

    if (argc != 4) {
        printf("Not equal args in user client\n");
        return ret;
    }

    ret.role = USER_CLIENT;
    ret.saddr = argv[2];
    ret.sport = argv[3];

    return ret;
}

notapp_args parse_args(int argc, char *argv[])
{
    // assumes that len(argv) == argc
    if (argc < MIN_ARG_COUNT) {
        printf("todo: error handling in parse_args\n");
        return init_notapp_args();
    }

    const char *flag_role = argv[1];
    if (strcmp(flag_role, "-s") == 0) {
        return parse_args_server(argc, argv);
    } else if (strcmp(flag_role, "-o") == 0) {
        return parse_args_observer_client(argc, argv);
    } else if (strcmp(flag_role, "-u") == 0) {
        return parse_args_user_client(argc, argv);
    } else {
        printf("todo: error handling in parse_args\n");
        return init_notapp_args();
    }
}
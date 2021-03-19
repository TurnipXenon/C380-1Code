#include "argparser.h"

/**
 * @brief Creates a default struct notapp_args
 * 
 * @return notapp_args 
 */
notapp_args init_notapp_args() {
    notapp_args ret = {
        UNKNOWN, // role
        5.0f, // interval
        "0", // sport
        NULL, // logfile
        "127.0.0.1", // saddr
        "randomfile.txt" // fileordir
    };
    return ret;
}

/**
 * @brief Specialized parser for server types
 * 
 * @param argc 
 * @param argv 
 * @return notapp_args 
 */
static notapp_args parse_args_server(int argc, char *argv[]) {
    notapp_args ret = init_notapp_args();

    if (argc < 3) {
        printf("Not enough args\n");
        return ret;
    }

    int index = 2;
    bool interval_flag = false;

    while (index < argc) {
        if (index + 1 == argc) {
            printf("Invalid argument format\n");
            return ret;
        }

        if (strcmp(argv[index], "-t") == 0) {
            char *pf;
            ret.interval = strtof(argv[index + 1], &pf);
            interval_flag = true;
        } else if (strcmp(argv[index], "-p") == 0) {
            ret.sport = argv[index + 1];
        } else if (strcmp(argv[index], "-l") == 0) {
            ret.logfile = argv[index + 1];
        } else {
            printf("Unknown flag detected: %s\n", argv[index]);
            return ret;
        }

        index += 2;
    }

    if (!interval_flag) {
        printf("Required flag not present\n");
        return ret;
    }

    ret.role = SERVER;

    return ret;
}

/**
 * @brief Specialized parser for observer client types
 * 
 * @param argc 
 * @param argv 
 * @return notapp_args 
 */
static notapp_args parse_args_observer_client(int argc, char *argv[]) {
    notapp_args ret = init_notapp_args();

    if (argc != 5) {
        printf("Not enough args\n");
        return ret;
    }

    ret.role = OBSERVER_CLIENT;
    ret.saddr = argv[2];
    ret.sport = argv[3];
    ret.fileordir = argv[4];

    return ret;
}

/**
 * @brief Specialized parser for user client types
 * 
 * @param argc 
 * @param argv 
 * @return notapp_args 
 */
static notapp_args parse_args_user_client(int argc, char *argv[]) {
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

/**
 * @brief Parses the command line arguments and interprets them as a struct notapp_args
 * 
 * @param argc 
 * @param argv 
 * @return notapp_args 
 */
notapp_args parse_args(int argc, char *argv[])
{
    // assumes that len(argv) == argc
    if (argc < MIN_ARG_COUNT) {
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
        return init_notapp_args();
    }
}
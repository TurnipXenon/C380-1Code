#include "argparser.h"

void print_notapp_args(notapp_args *arg) {
    printf("role: %d\n", arg->role);
    printf("interval: %f\n", arg->interval);
    printf("sport: %s\n", arg->sport);
    printf("logfile: %s\n", arg->logfile);
    printf("saddr: %s\n", arg->saddr);
    printf("fileordir: %s\n", arg->fileordir);
}

void test_init_notapp_args() {
    printf("Testing notapp args initialization\n");
    notapp_args arg = init_notapp_args();
    print_notapp_args(&arg);
    printf("Finish testing notapp args initialization\n\n");
}

void test_parse_args() {
    notapp_args val1 = parse_args(
        8, 
        (char *[]){"notapp", "-s", "-t", "7.5", "-p", "111.111.111.111", "-l", "random.txt"}
    );
    print_notapp_args(&val1);
    notapp_args val2 = parse_args(
        5, 
        (char *[]){"notapp", "-o", "7777.7777.7777.777", "1111.1111.111.0", "fileordir.txt"}
    );
    print_notapp_args(&val2);
    notapp_args val3 = parse_args(
        4, 
        (char *[]){"notapp", "-u", "222.222.22.2", "33.3.3.3"}
    );
    print_notapp_args(&val3);
}

void test_argparser() {
    // test_init_notapp_args();
    test_parse_args();
}

int main() {
    test_argparser();
}
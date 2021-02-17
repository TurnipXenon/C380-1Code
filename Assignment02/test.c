#include "argparser.h"

void print_notapp_args(notapp_args *arg) {
    printf("%d\n", arg->role);
    printf("%f\n", arg->interval);
    printf("%s\n", arg->sport);
    printf("%s\n", arg->logfile);
}

void test_init_notapp_args() {
    printf("Testing notapp args initialization\n");
    notapp_args arg = init_notapp_args();
    print_notapp_args(&arg);
    printf("Finish testing notapp args initialization\n\n");
}

void test_parse_args() {
    notapp_args val1 = parse_args(8, (char *[]){"notapp", "-s", "-t", "7.5", "-p", "111.111.111.111", "-l", "random.txt"});
    print_notapp_args(&val1);
}

void test_argparser() {
    // test_init_notapp_args();
    test_parse_args();
}

int main() {
    test_argparser();
}
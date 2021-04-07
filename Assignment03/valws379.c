#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief 
 * 
 * whatever 2> asdfa.txt
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char *argv[]) {
    unsigned int skipsize = 0u;
    char str_line[5];

    if (argc == 3 && strcmp(argv[1], "-s") == 0) {
        skipsize = (unsigned int) atoi(argv[2]);
        // todo: put precautions of negative value
        printf("skipsize: %d\n", skipsize);

        // todo: additional powers of two check
        // todo: check if less than
    } else if (argc != 0) {
        printf("todo: Arg error\n");
        return 0;
    }

    // /* Start read once you see I in char[0] */


    // printf("Hello: %s\n", str_line);
    fprintf(stdout, "Say something!\n");

    /* End reading when you see = */

    return 0;
}
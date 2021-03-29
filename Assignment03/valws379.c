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

    return 0;
}
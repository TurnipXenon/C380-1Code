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
int main(int argc, char *argv[])
{
    unsigned int skipsize = 0u;
    char str_line[41];

    /* from: https://linuxhint.com/pipe_system_call_c/ */
    // int pipefds[2];

    // if(pipe(pipefds) == -1) {
    //     perror("pipe");
    //     exit(EXIT_FAILURE);
    // }

    if (argc == 3 && strcmp(argv[1], "-s") == 0)
    {
        skipsize = (unsigned int)atoi(argv[2]);
        // todo: put precautions of negative value
        printf("skipsize: %d\n", skipsize);

        // todo: additional powers of two check
        // todo: check if less than
    }
    else if (argc != 0)
    {
        printf("todo: Arg error\n");
        return 0;
    }

    // /* Start read once you see I in char[0] */
    // read(pipefds[0], str_line, 4);
    scanf("%40s", str_line);

    /* Scanf until empty: https://stackoverflow.com/questions/27607744/reading-input-from-stdin-until-empty-line */
    FILE *fptr = NULL;
    fptr = fopen("newprogram.txt", "w");
    if (fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }

    printf("Okie dokie: %s\n", str_line);
    scanf("%40s", str_line);

    printf("Okie dokie 2: %s\n", str_line);
    fclose(fptr);

    /* End reading when you see = */

    return 0;
}
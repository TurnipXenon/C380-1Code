#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // getopt
#include <libgen.h> // basename
#include <ctype.h> /* for isspace */
#include <stdbool.h>

#include "windowset.h"

#define BUFFER_SIZE 1000

/**
 * @brief 
 * 
 * @remark
 * From Assignment 2 getopt example
 * 
 * @param program_name 
 */
void print_usage(char *program_name) {
    printf("Usage %s [-s <skipsize>] pgsize windowsize\n", basename(program_name));
}

/**
 * @brief from https://stackoverflow.com/a/27607770/10024566
 * 
 * @param line 
 * @return bool 
 */
bool is_empty_line(const char *line)
{
    /* check if the string consists only of spaces. */
    while (*line != '\0')
    {
        if (isspace(*line) == 0) {
            return true;
        }

        line++;
    }

    return false;
}

bool is_valid_code(const char* key) {
    const char * valid_codes[] = {
        "I",
        "S",
        "L",
        "M"
    };
    
    for (int i = 0; i < 4; ++i) {
        if (strcmp(key, valid_codes[i]) == 0) {
            return true;
        }
    }

    return false;
}

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
    // todo: do arguments
    ull skipsize = 0u;
    ull page_size = 0u;
    ull window_size = 0u;

    ull ref_address = 0u;
    ull ref_page_size = 0u;

    char str_buffer[BUFFER_SIZE + 1];
    char char_code[5];
    char str_numbers[101];
    char *token;
    char seps[] = ",";

    /* From get opt example */
    int c;

    // call getopt until all options are proccessed
    while ((c = getopt(argc, argv, "s:h")) != -1) {
        switch (c) {
            case 's':
                skipsize = strtoull(optarg, NULL, 10);
                // todo: put precautions of negative value
                // printf("skipsize: %llu\n", skipsize);

                // todo: additional powers of two check
                // todo: check if less than
                break;

            case 'h':
                print_usage(argv[0]);
                return 0;
            case '?':
                // option not in optstring or error
                break;
        }
    }
    
    if (optind + 1 < argc) {
        page_size = strtoull(argv[optind], NULL, 10);
        window_size = strtoull(argv[optind + 1], NULL, 10);
    } else {
        printf("Not enough arguments\n");
        print_usage(argv[0]);
        return 1;
    }

    initialize_window_set(page_size, window_size);

    // /* Start read once you see I in char[0] */
    /* from: https://stackoverflow.com/a/27607770/10024566 */
    ull mem_ref_count = 0u;
    int num = 0; /* todo: delete */
    while(fgets(str_buffer, BUFFER_SIZE, stdin) != NULL && is_empty_line(str_buffer)) {
        if (sscanf(str_buffer, "%4s %100s", char_code, str_numbers) == 2) {
            // printf("%s %s\n", char_code, str_numbers);

            if (is_valid_code(char_code)) {
                /* todo: skip references here */
                if (mem_ref_count < skipsize) {
                    ++mem_ref_count;
                    continue;
                }

                /* 
                Assume that if we get the correct code, we have the correct format after
                from: https://stackoverflow.com/a/1483218/10024566
                comma separation here */
                
                token = strtok(str_numbers, seps);
                ref_address = strtoull(token, NULL, 16);
                token = strtok(NULL, seps);
                sscanf(token, "%llu", &ref_page_size);
                
                /* todo: insert stuff here */
                window_set_insert(ref_address, ref_page_size);

                // printf("Address: %llX  ;  Page size: %llu\n", ref_address, ref_page_size);
                printf("%llu\n", get_window_set_size());
                // printf("%s, %llX, %llu, %llu\n", char_code, ref_address, ref_page_size, get_window_set_size());
                // window_set_debug();
                
                // if (num > 1000) {
                //     break;
                // }

                // ++num;
            }

        }
    }

    destroy_window_set();



    // read_result = fgets(str_buffer, BUFFER_SIZE, stdin);
    // printf("Result: %s\n", str_buffer);
    // read(pipefds[0], str_line, 4);
    // scanf("%40s", str_line);

    /* Skip skipsize memory references */

    /* End reading when you see = */

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> /* for getopt */
#include <libgen.h> /* for basename */
#include <ctype.h> /* for isspace */
#include <stdbool.h>

#include "windowset.h"

#define BUFFER_SIZE 1000

#define DEBUG_START "# Start\n"
#define DEBUG_END "# End\n"
// #define DEBUG_SKIP_PRINT 63

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
                skipsize = strtoull(optarg, NULL, 10) + 1; // adjust cause non-zero start
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

    #ifdef DEBUG_START
    bool is_start = false;
    #endif

    #ifdef DEBUG_SKIP_PRINT
    ull skips = 0;
    #endif

    /* from: https://stackoverflow.com/a/27607770/10024566 */
    ull mem_ref_count = 0u;
    while(fgets(str_buffer, BUFFER_SIZE, stdin) != NULL && is_empty_line(str_buffer)) {
        if (sscanf(str_buffer, "%4s %100s", char_code, str_numbers) == 2) {
            // printf("%s %s\n", char_code, str_numbers);

            if (is_valid_code(char_code)) {
                /* todo: skip references here */
                ++mem_ref_count;
                if (mem_ref_count < skipsize) {
                    continue;
                }

                #ifdef DEBUG_START
                if (!is_start) {
                    continue;
                }
                #endif 

                #ifdef DEBUG_SKIP_PRINT

                /* For clarity */
                bool shouldSkip = skips != 0;

                ++skips;

                if (shouldSkip) {

                    if (skips >= DEBUG_SKIP_PRINT) {
                        skips = 0; // reset
                    }

                    continue;
                }

                #endif

                /* from: https://stackoverflow.com/a/1483218/10024566 */
                token = strtok(str_numbers, seps);
                ref_address = strtoull(token, NULL, 16);
                token = strtok(NULL, seps);
                sscanf(token, "%llu", &ref_page_size);
                
                #ifdef DEBUG_PRINT
                printf("\n\n%s, %llX, %llu, %llu\n", char_code, ref_address, ref_page_size, get_window_set_size());
                #endif /* DEBUG_PRINT */

                window_set_insert(ref_address, ref_page_size);

                printf("%llu, %llu\n", mem_ref_count, get_window_set_size());
                #ifdef DEBUG_PRINT
                window_set_debug();
                #endif /* DEBUG_PRINT */
                
                #ifdef DEBUG_LIMIT
                if (mem_ref_count > DEBUG_LIMIT) {
                    break;
                }
                #endif /* DEBUG_LIMIT */
            }

            #ifdef DEBUG_COMMENTS
            if (strcmp(char_code, "#") == 0) {
                printf("%s", str_buffer);
            }
            #endif

            #ifdef DEBUG_START
            if (!is_start && strcmp(str_buffer, DEBUG_START) == 0) {
                is_start = true;
            }
            #endif

            #ifdef DEBUG_END
            if (strcmp(str_buffer, DEBUG_END) == 0) {
                break;
            }
            #endif
        }
    }

    destroy_window_set();

    return 0;
}
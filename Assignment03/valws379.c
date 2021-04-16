/**
 * @file valws379.c
 * @brief Prints the windowset size as input is being streamed into it
 * 
 * @remark define DEBUG_START "%s\n" string if you want to start printing only once
 * "%s\n" is printed
 * @remark define DEBUG_END "%s\n" string if you want to stop printing only once
 * "%s\n" is printed
 * @remark define DEBUG_SKIP_PRINT unsigned long long if you want to skip SKIP_PRINT windowset size
 * prints once you print a windowset size. Useful for very big datasets.
 * @remark define DEBUG_COMMENTS if you want to also want to print comments or string 
 * inputs that start with #
 * @remark define DEBUG_PRINT if you want to print details about the memory reference,
 * and other debug logging features for windowset
 * @remark define DEBUG_LIMIT unsigned long long if you want to have numeric limit for 
 * the windowset sizes printed. It's like skipsizes but on the other end.
 */

#include <stdio.h> 
#include <stdlib.h>

#include "windowset.h"
#include "valws379_base.h"

#define BUFFER_SIZE 1000

int main(int argc, char *argv[])
{
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

    /* call getopt until all options are proccessed */
    while ((c = getopt(argc, argv, "s:h")) != -1) {
        switch (c) {
            case 's':
                skipsize = strtoull(optarg, NULL, 10) + 1;
                break;

            case 'h':
                print_usage(argv[0]);
                return 0;
            case '?':
                /* option not in optstring or error */
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
    #endif /* DEBUG_START */

    #ifdef DEBUG_SKIP_PRINT
    ull skips = 0;
    #endif /* DEBUG_SKIP_PRINT */

    /* from: https://stackoverflow.com/a/27607770/10024566 */
    ull mem_ref_count = 0u;
    while(fgets(str_buffer, BUFFER_SIZE, stdin) != NULL && is_empty_line(str_buffer)) {
        if (sscanf(str_buffer, "%4s %100s", char_code, str_numbers) == 2) {

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
                #endif /* DEBUG_START */

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
                #endif /* DEBUG_SKIP_PRINT */

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
            #endif /* DEBUG_COMMENTS */

            #ifdef DEBUG_START
            if (!is_start && strcmp(str_buffer, DEBUG_START) == 0) {
                is_start = true;
            }
            #endif /* DEBUG_START */

            #ifdef DEBUG_END
            if (strcmp(str_buffer, DEBUG_END) == 0) {
                break;
            }
            #endif /* DEBUG_END */
        }
    }

    destroy_window_set();

    return 0;
}
#include "valws379_base.h"

/**
 * @brief Create new mem_ref
 * 
 * @return struct mem_ref 
 */
struct mem_ref new_mem_ref() {
    struct mem_ref mem_ref;
    mem_ref.address = 0u;
    mem_ref.page_count = 0u;
    return mem_ref;
}

/**
 * @brief Get the power of two for a value.
 * Assumes that value is > 0
 * 
 * @param value 
 * @return ull 
 */
ull get_power_of_two(ull value) {
    ull exponent = -1;
    ull new_value = value;
    
    while (new_value != 0) {
        ++exponent;
        new_value = new_value >> 1;
    }

    return exponent;
}

/**
 * @brief Print usage instruction
 * 
 * @remark from Assignment 2 getopt example
 * 
 * @param program_name or argv[0]
 */
void print_usage(char *program_name) {
    printf("Usage %s [-s <skipsize>] pgsize windowsize\n", basename(program_name));
}

/**
 * @brief Checks if line is empty
 * 
 * @remark from https://stackoverflow.com/a/27607770/10024566
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

/**
 * @brief Checks if key is either I, S, M, or L.
 * 
 * @param key 
 * @return true
 * @return false 
 */
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
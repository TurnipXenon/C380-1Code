#include <stdio.h>
#include <stdlib.h>

// gcc -g example2.c && valgrind ./a.out

int main(void) {
  int *some_numbers = malloc(sizeof(*some_numbers) * 2);
  some_numbers[0] = 1;
  some_numbers[1] = 3;
  free(some_numbers);
  some_numbers[0] = 2;  // <-- use after free!
  return 0;
}

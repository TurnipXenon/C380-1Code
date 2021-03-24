#include <stdio.h>
#include <stdlib.h>

// gcc -g example3.c && valgrind ./a.out

int main(void) {
  int *some_numbers = malloc(sizeof(*some_numbers) * 2);
  some_numbers[0] = 1;
  some_numbers[1] = 3;
  some_numbers[2] = 100;  // <-- not malloc'ed!
  some_numbers[3] = 100;  // <-- not malloc'ed!
  free(some_numbers);
  return 0;
}

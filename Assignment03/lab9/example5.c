#include <stdio.h>
#include <stdlib.h>

// gcc -g example5.c && valgrind ./a.out

int main(void) {
  int *x = malloc(sizeof(*x));
  // ...
  x = 0x00000123;  // at some point our pointer might get set to something else
  // ...
  *x = 56;  // <-- accessing memory that's not ours
  free(x);
  return 0;
}

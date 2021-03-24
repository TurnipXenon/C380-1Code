#include <stdio.h>
#include <stdlib.h>

// gcc -g example1.c && valgrind ./a.out

int main(void) {
  int a;
  printf("%d\n", a);  // <-- uninitialized memory!
  return 0;
}

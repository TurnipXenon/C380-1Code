#include <stdio.h>
#include <stdlib.h>

// gcc -g lackey.c && valgrind --tool=lackey --trace-mem=yes ./a.out

int main(void) {
  printf("# Testing start\n");
  int x = 0;
  if (x) {
    x = 1;
  }
  else {
    x = 2;
  }
  printf("# Testing end\n");
  return 0;
}

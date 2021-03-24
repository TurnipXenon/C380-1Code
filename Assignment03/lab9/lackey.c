#include <stdio.h>
#include <stdlib.h>

// gcc -g lackey.c && valgrind --tool=lackey --trace-mem=yes ./a.out

int main(void) {
  int x = 0;
  if (x) {
    x = 1;
  }
  else {
    x = 2;
  }
  return 0;
}

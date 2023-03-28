#include <test.h>

#include <stdlib.h>
#include <stdio.h>

void test() {
  printf("Hello World!\n");

  int* x = (int*)malloc(sizeof(int));

  *x = 7;

  printf("%d\n", *x);

  x = (int*)calloc(3, sizeof(int));
  x[0] = 7;
  x[1] = 77;
  x[2] = 777;

  printf("%d %d %d\n", x[0], x[1], x[2]);
}

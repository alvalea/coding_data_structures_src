#include <algo/container_test.h>

#include <stdio.h>
#include <stdlib.h>
#include <algo/container.h>

#define STR 10

typedef char string[STR];

static
void test_Heap_print(void* value) {
  int* v = (int*)value;
  printf("%d ",*v);
}

int test_Heap_push() {
  int result = 0;
  int n = 10;
  Heap* h = new_Heap(sizeof(int), sizeof(int), n);
  {
    for (int i = 1; i<=n; ++i) {
      Heap_push(h, &i, &i);
    }
    for (int i = n; i>0; --i) {
      int x;
      Heap_pop(h, &x);
      if (x != i) {
        result = -1;
      }
    }
  }
  delete_Heap(h);
  return result;
}

int test_Heap_pop() {
  int result = 0;
  string str;
  int n = 10;
  Heap* h = new_Heap(sizeof(string), sizeof(int), n);
  {
    for (int i = 1; i<=n; ++i) {
      _itoa_s(i, str, STR, 10);
      Heap_push(h, &i, str);
    }
    for (int i = n; i>0; --i) {
      Heap_pop(h, str);
      int x = atoi(str);
      if (x != i) {
        result = -1;
      }
    }
  }
  delete_Heap(h);
  return result;
}


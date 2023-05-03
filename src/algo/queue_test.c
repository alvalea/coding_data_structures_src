#include <algo/container_test.h>

#include <stdio.h>
#include <algo/container.h>

static
void test_Queue_print(void* item) {
  int* i = (int*)item;
  printf("%d ", *i);
}

int test_Queue_push() {
  int result = 0;
  Queue* q = new_Queue(sizeof(int), 1);
  {
    int i = 3;
    Queue_push(q, &i);
    int x;
    Queue_pop(q, &x);
    if (x != i) {
      result = -1;
    }
  }
  delete_Queue(q);
  return result;
}

int test_Queue_pop() {
  int result = 0;
  int n = 5;
  Queue* q = new_Queue(sizeof(int), n);
  {
    for (int i = 0; i<n; ++i) {
      Queue_push(q, &i);
    }
    for (int i = 0; i<n/2; ++i) {
      int x;
      Queue_pop(q, &x);
      if (x != i) {
        result = -1;
      }
    }
  }
  delete_Queue(q);
  return result;
}

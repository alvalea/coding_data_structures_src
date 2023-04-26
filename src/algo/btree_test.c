#include <algo/container_test.h>

#include <algo/container.h>
#include <stdio.h>

static
void test_BTree_print(void* value) {
  int* v = (int*)value;
  printf("%d.",*v);
}

int test_BTree_insert() {
  int result = 0;
  BTree* t = new_BTree(2);
  {
    for (int i=1; i<=10; ++i) {
      BTree_insert(t, i);
    }
    printf("\n");
    BTree_print(t, test_BTree_print);
    BTree_delete(t, 3);
    printf("\n");
    BTree_print(t, test_BTree_print);
  }
  delete_BTree(t);
  return result;
}

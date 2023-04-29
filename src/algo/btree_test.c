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

  for (int i=2; i<6; ++i) {
    BTree* t = new_BTree(i);
    {
      for (int j=1; j<=50; ++j) {
        BTree_insert(t, j);
      }
      printf("\n");
      BTree_print(t, test_BTree_print);
      for (int k=1; k<=50; ++k) {
        if (k%3== 0) {
          BTree_remove(t, k);
        }
      }
      printf("\n");
      BTree_print(t, test_BTree_print);
    }
    delete_BTree(t);
  }
  return result;
}

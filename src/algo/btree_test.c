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
      for (int j=1; j<=50; ++j) {
        if (j%3== 0) {
          BTree_remove(t, j);
        }
      }
    }
    delete_BTree(t);
  }
  return result;
}

int test_BTree_find() {
  int result = 0;
  BTree* t = new_BTree(5);
  {
    for (int j=1; j<=50; ++j) {
      BTree_insert(t, j);
    }
    for (int j=1; j<=50; ++j) {
      if (j%3== 0) {
        BTree_remove(t, j);
      }
    }

    int x = BTree_find(t, 22);
    if (x != 22) {
      result = -1;
    }
  }
  delete_BTree(t);
  return result;
}

#include <algo/container_test.h>

#include <algo/container.h>
#include <stdio.h>

static
int test_BTree_compare(void* key1, void* key2) {
  int k1 = *(int*)key1;
  int k2 = *(int*)key2;
  return k1 - k2;
}

static
void test_BTree_print(void* value) {
  int* v = (int*)value;
  printf("%d.",*v);
}

int test_BTree_insert() {
  int result = 0;
  for (int i=2; i<6; ++i) {
    BTree* t = new_BTree(i, sizeof(int), sizeof(int), test_BTree_compare);
    {
      for (int j=1; j<=50; ++j) {
        BTree_insert(t, &(int){j}, &(int){j});
      }
      for (int j=1; j<=50; ++j) {
        if (j%3== 0) {
          BTree_remove(t, &(int){j});
        }
      }
    }
    delete_BTree(t);
  }
  return result;
}

int test_BTree_find() {
  int result = 0;
  BTree* t = new_BTree(2, sizeof(int), sizeof(int), test_BTree_compare);
  {
    for (int j=1; j<=50; ++j) {
      BTree_insert(t, &(int){j}, &(int){j});
    }
    for (int j=1; j<=50; ++j) {
      if (j%3== 0) {
        BTree_remove(t, &(int){j});
      }
    }
    int* x = BTree_find(t, &(int){22});
    if (*x != 22) {
      result = -1;
    }
  }
  delete_BTree(t);
  return result;
}

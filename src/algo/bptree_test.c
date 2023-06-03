#include <algo/container_test.h>

#include <algo/container.h>

int test_BpTree_insert() {
  int result = 0;
  for (int i=3; i<6; ++i) {
    BpTree* t = new_BpTree(i);
    {
      for (int j=1; j<=50; ++j) {
        BpTree_insert(t, j, j);
      }
      for (int j=1; j<=50; ++j) {
        if (j%3== 0) {
          BpTree_remove(t, j);
        }
      }
      BpTree_print(t);
    }
    delete_BpTree(t);
  }
  return result;
}

int test_BpTree_find() {
  return -1;
}

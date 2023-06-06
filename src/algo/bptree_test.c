#include <algo/container_test.h>

#include <algo/container.h>

static
void test_BpTree_print(void* value) {
  int* v = (int*)value;
  printf("%d.", *v);
}

int test_BpTree_insert() {
  int result = 0;
  for (int i=3; i<6; ++i) {
    BpTree* t = new_BpTree(i, sizeof(int));
    {
      for (int j=50; j>0; --j) {
        BpTree_insert(t, j, &(int){j});
      }
      for (int j = 50; j > 0; --j) {
        if (j%3== 0) {
          BpTree_remove(t, j);
        }
      }
    }
    delete_BpTree(t);
  }
  return result;
}

int test_BpTree_find() {
  int result = 0;
  for (int i=3; i<6; ++i) {
    BpTree* t = new_BpTree(i, sizeof(int));
    {
      for (int j=1; j<=50; ++j) {
        BpTree_insert(t, j, &(int){j});
      }
      for (int j=1; j<=50; ++j) {
        if (j%3== 0) {
          BpTree_remove(t, j);
        }
      }
    }

    int* x = (int*)BpTree_find(t, 20);
    if (*x != 20) {
      result = -1;
    }
    delete_BpTree(t);
  }
  return result;

}

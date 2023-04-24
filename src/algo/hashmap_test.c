#include <algo/container_test.h>

#include <stdlib.h>
#include <algo/container.h>

static
bool test_HashMap_equal(void* key1, void* key2) {
  int* k1 = (int*)key1;
  int* k2 = (int*)key2;
  return *k1 == *k2;
}

int test_HashMap_insert() {
  int result = 0;
  HashMap* m = new_HashMap(sizeof(int), sizeof(int), test_HashMap_equal);
  {
    int i = 3;
    HashMap_insert(m, &i, &i);
    int* x = HashMap_find(m, &i);
    if (*x != i){
      result = -1;
    }
  }
  delete_HashMap(m);
  return result;
}

int test_HashMap_delete() {
  int result = 0;
  HashMap* m = new_HashMap(sizeof(int), sizeof(int), test_HashMap_equal);
  {
    int i = 3;
    HashMap_insert(m, &i, &i);
    HashMap_delete(m, &i);
    int* x = HashMap_find(m, &i);
    if (x != NULL){
      result = -1;
    }
  }
  delete_HashMap(m);
  return result;
}

int test_HashMap_find() {
  int result = 0;
  HashMap* m = new_HashMap(sizeof(int), sizeof(int), test_HashMap_equal);
  {
    for (int i=0; i<100; ++i) {
      HashMap_insert(m, &i, &i);
    }
    int i = 99;
    int* x = HashMap_find(m, &i);
    if (*x != i){
      result = -1;
    }
  }
  delete_HashMap(m);
  return result;
}

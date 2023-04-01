#include <algo/container_test.h>

#include <stdlib.h>
#include <algo/container.h>

int test_HashMap_insert() {
  int result = 0;
  HashMap* m = new_HashMap(sizeof(int), sizeof(int));
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
  HashMap* m = new_HashMap(sizeof(int), sizeof(int));
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
  HashMap* m = new_HashMap(sizeof(int), sizeof(int));
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

#include <algo/container_test.h>

#include <stdio.h>
#include <stdlib.h>
#include <algo/container.h>

#define STR 10

typedef char string[STR];

static
void test_Map_print(void* value) {
  int* v = (int*)value;
  printf("%d\n",*v);
}

int test_Map_insert() {
  int result = 0;
  Map* m = new_Map(sizeof(string), sizeof(int));
  {
    int i = 3;
    string str;
    _itoa_s(i, str, STR, 10);
    Map_insert(m, str, &i);
    int* x = (int*)Map_find(m, str);
    if (*x != i) {
      result = -1;
    }
  }
  delete_Map(m);
  return result;
}

int test_Map_delete() {
  int result = 0;
  Map* m = new_Map(sizeof(string), sizeof(int));
  {
    int i = 3;
    string str;
    _itoa_s(i, str, STR, 10);
    Map_insert(m, str, &i);
    Map_delete(m, str);
    if (Map_find(m, str) != NULL) {
      result = -1;
    }
  }
  delete_Map(m);
  return result;
}

int test_Map_find() {
  int result = 0;
  Map* m = new_Map(sizeof(string), sizeof(int));
  {
    int i = 3;
    string str;
    _itoa_s(i, str, STR, 10);
    Map_insert(m, str, &i);
    int* x = (int*)Map_find(m, str);
    if (*x != i) {
      result = -1;
    }
  }
  delete_Map(m);
  return result;
}

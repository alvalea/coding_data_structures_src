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

static
int test_Map_compare(void* key1, void* key2) {
  int i1 = atoi(key1);
  int i2 = atoi(key2);
  if (i1 < i2) {
    return -1;
  } else if (i1 > i2) {
    return 1;
  } else {
    return 0;
  }
}

int test_Map_insert() {
  int result = 0;
  Map* m = new_Map(sizeof(string), sizeof(int), test_Map_compare);
  {
    for (int i=0; i<10; ++i) {
      string str;
      sprintf(str, "%d", i);
      Map_insert(m, str, &i);
      int* x = (int*)Map_find(m, str);
      if (*x != i) {
        result = -1;
      }
    }
    //Map_print(m, test_Map_print);
  }
  delete_Map(m);
  return result;
}

int test_Map_remove() {
  int result = 0;
  Map* m = new_Map(sizeof(string), sizeof(int), test_Map_compare);
  {
    int i = 3;
    string str;
    sprintf(str, "%d", i);
    Map_insert(m, str, &i);
    Map_remove(m, str);
    if (Map_find(m, str) != NULL) {
      result = -1;
    }
  }
  delete_Map(m);
  return result;
}

int test_Map_find() {
  int result = 0;
  Map* m = new_Map(sizeof(string), sizeof(int), test_Map_compare);
  {
    int i = 3;
    string str;
    sprintf(str, "%d", i);
    Map_insert(m, str, &i);
    int* x = (int*)Map_find(m, str);
    if (*x != i) {
      result = -1;
    }
  }
  delete_Map(m);
  return result;
}

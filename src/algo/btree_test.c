#include <algo/container_test.h>

#include <algo/container.h>
#include <stdio.h>
#include <stdlib.h>

#define STR 10
typedef char string[STR];

typedef struct Value {
  int number;
  string text;
} Value;

static
int test_BTree_compare(void* key1, void* key2) {
  Value* k1 = (Value*)key1;
  Value* k2 = (Value*)key2;
  return k1->number - k2->number;
}

static
int test_BTree_compare_int(void* key1, void* key2) {
  int k1 = *(int*)key1;
  int k2 = *(int*)key2;
  return k1 - k2;
}

static
void test_BTree_print(void* value) {
  Value* v = (Value*)value;
  printf("%s.",v->text);
}

int test_BTree_insert() {
  int result = 0;
  for (int i=2; i<6; ++i) {
    BTree* t = new_BTree(i, sizeof(Value), sizeof(Value), test_BTree_compare);
    {
      for (int j=1; j<=50; ++j) {
        Value v = { .number = j };
        sprintf(v.text, "%d", j);
        BTree_insert(t, &v, &v);
      }
      for (int j=1; j<=50; ++j) {
        if (j%3== 0) {
          Value v = { .number = j };
          sprintf(v.text, "%d", j);
          BTree_remove(t, &v);
        }
      }
    }
    delete_BTree(t);
  }
  return result;
}

int test_BTree_find() {
  int result = 0;
  BTree* t = new_BTree(2, sizeof(int), sizeof(int), test_BTree_compare_int);
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

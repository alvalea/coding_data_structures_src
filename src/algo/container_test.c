#include <algo/container_test.h>

#include <stdio.h>
#include <algo/container.h>

static
void test_Array_print(void* item) {
  int* i = (int*)item;
  printf("%d ", *i);
}

static
void test_List_print(void* item) {
  int* i = (int*)item;
  printf("%d ", *i);
}

int test_Array_add() {
  int result = 0;
  Array* a = new_Array(sizeof(int), 10);
  {
    Array_add(a, &(int){3});
    if (Array_len(a) != 1) {
      result = -1;
    }
  }
  delete_Array(a);
  return result;
}

int test_Array_remove() {
  int result = 0;
  Array* a = new_Array(sizeof(int), 10);
  {
    for (int i=1; i<=20; ++i) {
      Array_add(a, &i);
    }

    Array_remove(a, 3);
    Array_remove(a, 9);
    Array_remove(a, 15);

    if (Array_len(a) != 17) {
      result = 1;
    }
  }
  delete_Array(a);
  return result;
}

int test_Array_get() {
  int result = 0;
  Array* a = new_Array(sizeof(int), 10);
  {
    int i = 3;
    Array_add(a, &i);
    int* x = Array_get(a, 0);
    if (*x != i) {
      result = -1;
    }
  }
  delete_Array(a);
  return result;
}

int test_Array_len() {
  int result = 0;
  Array* a = new_Array(sizeof(int), 10);
  {
    Array_add(a, &(int){3});
    if (Array_len(a) != 1) {
      result = -1;
    }
  }
  delete_Array(a);
  return result;
}

int test_Array_clear() {
  int result = 0;
  Array* a = new_Array(sizeof(int), 10);
  {
    Array_add(a, &(int){3});
    Array_clear(a);
    if (Array_len(a) != 0) {
      result = -1;
    }
  }
  delete_Array(a);
  return result;
}

int test_List_add() {
  int result = 0;
  List* l = new_List(sizeof(int));
  {
    int i = 3;
    int* x = List_add(l, &i);
    if (*x != i) {
      result = -1;
    }
  }
  delete_List(l);
  return result;
}

int test_List_remove() {
  int result = 0;
  List* l = new_List(sizeof(int));
  {
    int* x = List_add(l, &(int){3});
    List_remove(l, x);
    if (List_head(l) != NULL) {
      result = -1;
    }
  }
  delete_List(l);
  return result;
}

int test_List_head() {
  int result = 0;
  List* l = new_List(sizeof(int));
  {
    int* x = List_add(l, &(int){3});
    ListNode* head = List_head(l);
    if (ListNode_data(head) != x) {
      result = -1;
    }
  }
  delete_List(l);
  return result;
}

int test_List_tail() {
  int result = 0;
  List* l = new_List(sizeof(int));
  {
    int* x = List_add(l, &(int){3});
    ListNode* tail = List_tail(l);
    if (ListNode_data(tail) != x) {
      result = -1;
    }
  }
  delete_List(l);
  return result;
}

int test_Queue_push() {
  int result = 0;
  Queue* q = new_Queue(sizeof(int), 1);
  {
    int i = 3;
    Queue_push(q, &i);
    int x;
    Queue_pop(q, &x);
    if (x != i) {
      result = -1;
    }
  }
  delete_Queue(q);
  return result;
}

int test_Queue_pop() {
  int result = 0;
  int n = 5;
  Queue* q = new_Queue(sizeof(int), n);
  {
    for (int i = 0; i<n; ++i) {
      Queue_push(q, &i);
    }
    for (int i = 0; i<n; ++i) {
      int x;
      Queue_pop(q, &x);
      if (x != i) {
        result = -1;
      }
    }
  }
  delete_Queue(q);
  return result;
}

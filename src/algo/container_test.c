#include <algo/container_test.h>

#include <test.h>
#include <stdio.h>
#include <algo/container.h>

int test_Array_add() {
  Array* a = new_Array(sizeof(int), 10);
  {
    int i = 3;
    Array_add(a, &i);
    if (Array_len(a) != 1) {
      return 1;
    }
  }
  delete_Array(a);
  return 0;
}

int test_Array_get() {
  Array* a = new_Array(sizeof(int), 10);
  {
    int i = 3;
    Array_add(a, &i);
    int* x = Array_get(a, 0);
    if (*x != i) {
      return 1;
    }
  }
  delete_Array(a);
  return 0;
}

int test_Array_len() {
  Array* a = new_Array(sizeof(int), 10);
  {
    int i = 3;
    Array_add(a, &i);
    if (Array_len(a) != 1) {
      return 1;
    }
  }
  delete_Array(a);
  return 0;
}

int test_Array_clear() {
  Array* a = new_Array(sizeof(int), 10);
  {
    int i = 3;
    Array_add(a, &i);
    Array_clear(a);
    if (Array_len(a) != 0) {
      return 1;
    }
  }
  delete_Array(a);
  return 0;
}

int test_List_add() {
  List* l = new_List(sizeof(int));
  {
    int i = 3;
    int* x = List_add(l, &i);
    if (*x != i) {
      return 1;
    }
  }
  delete_List(l);
  return 0;
}

int test_List_remove() {
  List* l = new_List(sizeof(int));
  {
    int i = 3;
    int* x = List_add(l, &i);
    List_remove(l, x);
    if (List_head(l) != NULL) {
      return 1;
    }
  }
  delete_List(l);
  return 0;
}

int test_List_head() {
  List* l = new_List(sizeof(int));
  {
    int i = 3;
    int* x = List_add(l, &i);
    ListNode* head = List_head(l);
    if (ListNode_data(head) != x) {
      return 1;
    }
  }
  delete_List(l);
  return 0;
}

int test_List_tail() {
  List* l = new_List(sizeof(int));
  {
    int i = 3;
    int* x = List_add(l, &i);
    ListNode* tail = List_tail(l);
    if (ListNode_data(tail) != x) {
      return 1;
    }
  }
  delete_List(l);
  return 0;
}

int test_List_find() {
  List* l = new_List(sizeof(int));
  {
    int i = 3;
    int* x = List_add(l, &i);
    ListNode* item = List_find(l, x);
    if (ListNode_data(item) != x) {
      return 1;
    }
  }
  delete_List(l);
  return 0;
}

int test_Queue_push() {
  Queue* q = new_Queue(sizeof(int), 1);
  {
    int i = 3;
    Queue_push(q, &i);
    int x;
    Queue_pop(q, &x);
    if (x != i) {
      return 1;
    }
  }
  delete_Queue(q);
  return 0;
}

int test_Queue_pop() {
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
        return 1;
      }
    }
  }
  delete_Queue(q);
  return 0;
}

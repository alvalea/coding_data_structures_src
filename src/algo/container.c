#include <algo/container.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Array {
  int item_size;
  int capacity;
  int len;
  void* items;
};

Array* new_Array(int item_size, int initial_capacity) {
  Array* a = calloc(1, sizeof(Array));

  if (initial_capacity == 0) {
    printf("Initial capacity should be higher than zero\n");
    exit(1);
  }

  a->item_size = item_size;
  a->capacity  = initial_capacity;
  a->items     = calloc(initial_capacity, item_size);

  return a;
}

void delete_Array(Array* a) {
  free(a->items);
  free(a);
}

static
void Array_resize(Array* a) {
  a->capacity *= 2;
  a->items = realloc(a->items, a->capacity * a->item_size);
}

int Array_add(Array* a, void* item) {
  if (a->len == a->capacity) {
    Array_resize(a);
  }
  int index = a->len;
  int offset = index * a->item_size;
  char* dst = (char*)a->items + offset;
  memcpy(dst, item, a->item_size);
  a->len++;
  return index;
}

void* Array_get(Array* a, int index) {
  if (index >= a->len) {
    printf("Index out of bounds: len %d, index %d\n", a->len, index);
    exit(1);
  }
  int offset = index * a->item_size;
  char* item = (char*)a->items + offset;
  return item;
}

int Array_len(Array* a) {
  return a->len;
}

void  Array_clear(Array* a) {
  a->len = 0;
}

//========================================================

struct ListNode {
  void* data;
  ListNode* next;
};

void* ListNode_data(ListNode* n) {
  return n->data;
}

ListNode* ListNode_next(ListNode* n) {
  return n->next;
}

static
void delete_ListNode(ListNode* n) {
    free(n->data);
    free(n);
}

struct List {
   int item_size;
   ListNode* head;
   ListNode* tail;
};

List* new_List(int item_size) {
  List* l = calloc(1, sizeof(List));
  l->item_size = item_size;
  return l;
}

void delete_List(List* l) {
  ListNode* n = l->head;
  while(n) {
    ListNode* next = n->next;
    delete_ListNode(n);
    n = next;
  }
  free(l);
}

void* List_add(List* l, void* item) {
  ListNode* n = calloc(1, sizeof(ListNode));
  n->data = malloc(l->item_size);
  memcpy(n->data, item, l->item_size);

  if (l->head == NULL) {
    l->head = n;
  }
  if (l->tail != NULL) {
    l->tail->next = n;
  }
  l->tail = n;
  return n->data;
}

void List_remove(List* l, void* item) {
  ListNode* node = List_find(l, item);
  ListNode* n = l->head;
  ListNode* prev = NULL;
  while(n != node) {
    prev = n;
    n = n->next;
  }
  if (l->head == node) {
    l->head = node->next;
  }
  if (l->tail == node) {
    l->tail = prev;
  }
  if (prev != NULL) {
    prev->next = node->next;
  }
  delete_ListNode(node);
}

ListNode* List_head(List* l) {
  return l->head;
}

ListNode* List_tail(List* l) {
  return l->tail;
}

ListNode* List_find(List* l, void* item) {
  ListNode* n = l->head;
  while(n) {
    if (memcmp(n->data, item, l->item_size) == 0) {
      return n;
    }
    n = n->next;
  }
  return NULL;
}


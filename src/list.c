#include <algo/container.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
   size_t item_size;
   ListNode* head;
   ListNode* tail;
};

List* new_List(size_t item_size) {
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

static
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

void List_print(List* l, ListPrintFn print) {
  ListNode* n = l->head;
  while(n) {
    print(n->data);
    n = n->next;
  }
}

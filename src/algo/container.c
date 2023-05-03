#include <algo/container.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Array {
  size_t item_size;
  size_t capacity;
  size_t len;
  void*  items;
};

Array* new_Array(size_t item_size, size_t capacity) {
  Array* a = calloc(1, sizeof(Array));

  if (capacity == 0) {
    printf("Initial capacity should be higher than zero\n");
    exit(1);
  }

  a->item_size = item_size;
  a->capacity  = capacity;
  a->items     = calloc(capacity, item_size);

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

size_t Array_add(Array* a, void* item) {
  if (a->len == a->capacity) {
    Array_resize(a);
  }
  size_t index = a->len;
  size_t offset = index * a->item_size;
  char* dst = (char*)a->items + offset;
  memcpy(dst, item, a->item_size);
  a->len++;
  return index;
}

void Array_remove(Array* a, size_t index) {
  if (index >= a->len) {
    printf("Index out of bounds: len %zu, index %zu\n", a->len, index);
    return;
  }
  size_t n = a->len - 1 - index;
  char* dst = (char*)a->items + (index * a->item_size);
  char* src = (char*)a->items + ((index + 1) * a->item_size);
  memmove(dst, src, a->item_size * n);
  a->len--;
}

void* Array_get(Array* a, size_t index) {
  if (index >= a->len) {
    printf("Index out of bounds: len %zu, index %zu\n", a->len, index);
    return NULL;
  }
  size_t offset = index * a->item_size;
  char* item = (char*)a->items + offset;
  return item;
}

size_t Array_len(Array* a) {
  return a->len;
}

void  Array_clear(Array* a) {
  a->len = 0;
}

void Array_print(Array* a, ArrayPrintFn print) {
  size_t len = Array_len(a);
  for (size_t i=0; i<len; ++i) {
    print(Array_get(a, i));
  }
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

//========================================================

struct Queue {
  size_t item_size;
  size_t front;
  size_t capacity;
  size_t count;
  void*  items;
};

Queue* new_Queue(size_t item_size, size_t capacity) {
  Queue* q = calloc(1, sizeof(Queue));

  if (capacity == 0) {
    printf("Initial capacity should be higher than zero\n");
    exit(1);
  }

  q->item_size = item_size;
  q->capacity  = capacity;
  q->items     = calloc(capacity, item_size);

  return q;
}

void delete_Queue(Queue* q) {
  free(q->items);
  free(q);
}

bool Queue_push(Queue* q, void* item) {
  if (q->count == q->capacity) {
    return false;
  }
  char* items = (char*)q->items;
  size_t index = (q->front + q->count) % q->capacity;
  char* dst = &items[index * q->item_size];
  memcpy(dst, item, q->item_size);
  q->count++;
  return true;
}

bool Queue_pop(Queue* q, void* item) {
  if (q->count == 0) {
    return false;
  }
  char* items = (char*)q->items;
  size_t index = q->front;
  char* src = &items[index * q->item_size];
  memcpy(item, src, q->item_size);
  q->count--;
  q->front = (q->front + 1) % q->capacity;
  return true;
}

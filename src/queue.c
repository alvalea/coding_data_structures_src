#include <algo/container.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

void Queue_print(Queue* q, QueuePrintFn print) {
  char* items = (char*)q->items;
  size_t index = q->front;
  for (int i=0; i<q->count; ++i) {
    print(&items[index * q->item_size]);
    index = (index + 1) % q->capacity;
  }
}

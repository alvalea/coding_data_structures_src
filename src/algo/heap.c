#include <algo/container.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Heap {
  size_t key_size;
  size_t value_size;
  size_t capacity;
  size_t count;
  void*  items;
};

Heap* new_Heap(size_t key_size, size_t value_size, size_t capacity) {
  Heap* h = calloc(1, sizeof(Heap));

  if (capacity == 0) {
    printf("Initial capacity should be higher than zero\n");
    exit(1);
  }

  h->key_size = key_size;
  h->value_size = value_size;
  h->capacity = capacity;
  h->items = calloc(capacity, key_size + value_size);

  return h;
}

void delete_Heap(Heap* h) {
  free(h->items);
  free(h);
}

static
bool Heap_bigger(Heap* h, size_t i, size_t j) {
  size_t item_size = h->key_size + h->value_size;
  char* items = (char*)h->items;
  char* item_i = &items[i * item_size];
  char* item_j = &items[j * item_size];
  return memcmp(item_i, item_j, h->key_size) > 0;
}

static
void Heap_swap(Heap* h, size_t i, size_t j) {
  size_t item_size = h->key_size + h->value_size;
  char* items = (char*)h->items;
  char* item_i = &items[i * item_size];
  char* item_j = &items[j * item_size];

  char* tmp = calloc(1, item_size);
  {
    memcpy(tmp, item_j, item_size);
    memcpy(item_j, item_i, item_size);
    memcpy(item_i, tmp, item_size);
  }
  free(tmp);
}

static
void Heap_heapify(Heap* h, size_t i) {
  size_t largest = i;
  size_t left = 2 * i + 1;
  size_t right = 2 * i + 2;
  if (left < h->count && Heap_bigger(h, left, largest)) {
    largest = left;
  }
  if (right < h->count && Heap_bigger(h, right, largest)) {
    largest = right;
  }
  if (largest != i) {
    Heap_swap(h, i, largest);
    Heap_heapify(h, largest);
  }
}

static
void Heap_sort(Heap* h) {
  if (h->count < 1) {
    return;
  }
  for (int i = ((int)h->count / 2) - 1; i >= 0; --i) {
    Heap_heapify(h, i);
  }
}

bool Heap_push(Heap* h, void* key, void* value) {
  if (h->count == h->capacity) {
    return false;
  }
  char* items = (char*)h->items;
  size_t index = h->count;
  size_t item_size = h->key_size + h->value_size;
  char* dst = &items[index * item_size];
  memcpy(dst, key, h->key_size);
  memcpy(dst + h->key_size, value, h->value_size);

  h->count++;
  Heap_sort(h);

  return true;
}

bool Heap_pop(Heap* h, void* value) {
  if (h->count == 0) {
    return false;
  }
  char* items = (char*)h->items;
  char* src = items + h->key_size;
  memcpy(value, src, h->value_size);

  Heap_swap(h, 0, h->count-1);

  h->count--;
  Heap_sort(h);

  return true;
}

void Heap_print(Heap* h, HeapPrintFn print) {
  for (size_t i=0; i<h->count; ++i) {
    char* items = (char*)h->items;
    size_t item_size = h->key_size + h->value_size;
    char* value = &items[i * item_size] + h->key_size;
    print(value);
  }
}

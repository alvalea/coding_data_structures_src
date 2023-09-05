#include <algo/container.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Heap {
        size_t item_size;
        HeapBiggerFn bigger;
        size_t capacity;
        size_t count;
        void*  items;
};

Heap* new_Heap(size_t item_size, size_t capacity, HeapBiggerFn bigger) {
        Heap* h = calloc(1, sizeof(Heap));

        if (capacity == 0) {
                printf("Initial capacity should be higher than zero\n");
                exit(1);
        }

        h->item_size = item_size;
        h->capacity = capacity;
        h->bigger = bigger;
        h->items = calloc(capacity, item_size);

        return h;
}

void delete_Heap(Heap* h) {
        free(h->items);
        free(h);
}

static
bool Heap_bigger(Heap* h, size_t i, size_t j) {
        char* items = (char*)h->items;
        char* item_i = &items[i * h->item_size];
        char* item_j = &items[j * h->item_size];
        if (h->bigger != NULL) {
                return h->bigger(item_i, item_j);
        }
        return memcmp(item_i, item_j, h->item_size) > 0;
}

static
void Heap_swap(Heap* h, size_t i, size_t j) {
        char* items = (char*)h->items;
        char* item_i = &items[i * h->item_size];
        char* item_j = &items[j * h->item_size];

        char* tmp = calloc(1, h->item_size);
        {
                memcpy(tmp, item_j, h->item_size);
                memcpy(item_j, item_i, h->item_size);
                memcpy(item_i, tmp, h->item_size);
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

bool Heap_push(Heap* h, void* item) {
        if (h->count == h->capacity) {
                return false;
        }
        char* items = (char*)h->items;
        size_t index = h->count;
        char* dst = &items[index * h->item_size];
        memcpy(dst, item, h->item_size);

        h->count++;
        Heap_sort(h);

        return true;
}

bool Heap_pop(Heap* h, void* item) {
        if (h->count == 0) {
                return false;
        }
        char* src = (char*)h->items;
        memcpy(item, src, h->item_size);

        Heap_swap(h, 0, h->count-1);

        h->count--;
        Heap_sort(h);

        return true;
}

void Heap_print(Heap* h, HeapPrintFn print) {
        for (size_t i=0; i<h->count; ++i) {
                char* items = (char*)h->items;
                char* item = &items[i * h->item_size];
                print(item);
        }
}

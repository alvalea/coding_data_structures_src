#include <algo/container.h>

#include <stdlib.h>
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

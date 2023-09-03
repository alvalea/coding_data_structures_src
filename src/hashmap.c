#include <algo/container.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static
const int CAPACITY = 32;

struct HashMap {
  size_t capacity;
  size_t key_size;
  size_t value_size;
  List** buckets;
  HashMapEqualFn equal;
};

HashMap* new_HashMap(size_t key_size, size_t value_size, HashMapEqualFn equal) {
  HashMap* m = calloc(1, sizeof(HashMap));
  m->capacity = CAPACITY;
  m->key_size = key_size;
  m->value_size = value_size;
  m->buckets = calloc(m->capacity, sizeof(List*));
  m->equal = equal;
  return m;
}

void delete_HashMap(HashMap* m) {
  for (int i=0; i<m->capacity; ++i) {
    if (m->buckets[i] != NULL) {
      delete_List(m->buckets[i]);
    }
  }
  free(m->buckets);
  free(m);
}

static
int HashMap_hash(HashMap* m, void* key) {
  int value = 0;
  for (int i=0; i<m->key_size; ++i) {
    char* v = (char*)key + i;
    value += *v;
  }
  return value % m->capacity;
}

static
void HashMap_insert_item(HashMap* m, int hash, void* key, void* value) {
  char* item = calloc(1, m->key_size + m->value_size);
  {
    memcpy(item, key, m->key_size);
    memcpy(item + m->key_size, value, m->value_size);
    List_add(m->buckets[hash], item);
  }
  free(item);
}

void HashMap_insert(HashMap* m, void* key, void* value) {
  int hash = HashMap_hash(m, key);
  if (m->buckets[hash] == NULL) {
    m->buckets[hash] = new_List(m->key_size + m->value_size);
  }
  HashMap_insert_item(m, hash, key, value);
}

static
bool HashMap_equal(HashMap* m, void* key1, void* key2) {
  if (m->equal != NULL) {
    return m->equal(key1, key2);
  }
  return memcmp(key1, key2, m->key_size) == 0;
}

void HashMap_remove(HashMap* m, void* key) {
  int hash = HashMap_hash(m, key);
  if (m->buckets[hash] != NULL) {
    ListNode* n = List_head(m->buckets[hash]);
    while (n) {
      void* item = ListNode_data(n);
      if (HashMap_equal(m, item, key)) {
        List_remove(m->buckets[hash], item);
        return;
      }
      n = ListNode_next(n);
    }
  }
}

void* HashMap_find(HashMap* m, void* key) {
  int hash = HashMap_hash(m, key);
  if (m->buckets[hash] != NULL) {
    ListNode* n = List_head(m->buckets[hash]);
    while (n) {
      void* item = ListNode_data(n);
      if (HashMap_equal(m, item, key)) {
        return (char*)item + m->key_size;
      }
      n = ListNode_next(n);
    }
  }
  return NULL;
}

void HashMap_print(HashMap* m, HashMapPrintFn print) {
  for (int i=0; i<m->capacity; ++i) {
    if (m->buckets[i] != NULL) {
      printf("[%d] ", i);
      ListNode* n = List_head(m->buckets[i]);
      while (n) {
        void* item = ListNode_data(n);
        print(item);
        printf(" -> ");
        n = ListNode_next(n);
      }
      printf("\n");
    }
  }
}

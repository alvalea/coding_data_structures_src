#include <algo/container.h>

#include <stdlib.h>
#include <string.h>

static
const int CAPACITY = 32;

struct HashMap {
  size_t capacity;
  size_t key_size;
  size_t value_size;
  List** buckets;
};

HashMap* new_HashMap(size_t key_size, size_t value_size) {
  HashMap* m = calloc(1, sizeof(HashMap));
  m->capacity = CAPACITY;
  m->key_size = key_size;
  m->value_size = value_size;
  m->buckets = calloc(m->capacity, sizeof(List*));
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

void HashMap_delete(HashMap* m, void* key) {
  int hash = HashMap_hash(m, key);
  if (m->buckets[hash] != NULL) {
    ListNode* n = List_head(m->buckets[hash]);
    while (n) {
      void* item = ListNode_data(n);
      if (memcmp(item, key, m->key_size) == 0) {
        List_remove(m->buckets[hash], item);
        return;
      }
    }
  }
}

void* HashMap_find(HashMap* m, void* key) {
  int hash = HashMap_hash(m, key);
  if (m->buckets[hash] != NULL) {
    ListNode* n = List_head(m->buckets[hash]);
    while (n) {
      void* item = ListNode_data(n);
      if (memcmp(item, key, m->key_size) == 0) {
        return (char*)item + m->key_size;
      }
    }
  }
  return NULL;
}

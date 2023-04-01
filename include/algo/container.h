#ifndef __ALGO_CONTAINER__
#define __ALGO_CONTAINER__

#include <stdbool.h>

typedef struct Array Array;

Array* new_Array(size_t item_size, size_t initial_capacity);
void delete_Array(Array* a);

size_t Array_add(Array* a, void* item);
void* Array_get(Array* a, size_t index);
size_t Array_len(Array* a);
void  Array_clear(Array* a);

//========================================================

typedef struct ListNode ListNode;

void* ListNode_data(ListNode* n);
ListNode* ListNode_next(ListNode* n);

typedef struct List List;

List* new_List(size_t item_size);
void delete_List(List* l);

void* List_add(List* l, void* item);
void List_remove(List* l, void* item);
ListNode* List_head(List* l);
ListNode* List_tail(List* l);
ListNode* List_find(List* l, void* item);

//========================================================

typedef struct Queue Queue;

Queue* new_Queue(size_t item_size, size_t capacity);
void delete_Queue(Queue* q);

bool Queue_push(Queue* q, void* item);
bool Queue_pop(Queue* q, void* item);

//========================================================

typedef struct Map Map;

Map* new_Map(size_t key_size, size_t value_size);
void delete_Map(Map* m);

void Map_insert(Map* m, void* key, void* value);
void Map_delete(Map* m, void* key);
void* Map_find(Map* m, void* key);

typedef void (*MapPrintFn)(void* value);
void Map_print(Map* m, MapPrintFn print);

//========================================================

typedef struct HashMap HashMap;

HashMap* new_HashMap(size_t key_size, size_t value_size);
void delete_HashMap(HashMap* m);

void HashMap_insert(HashMap* m, void* key, void* value);
void HashMap_delete(HashMap* m, void* key);
void* HashMap_find(HashMap* m, void* key);

#endif

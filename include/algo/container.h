#ifndef __ALGO_CONTAINER__
#define __ALGO_CONTAINER__

#include <stddef.h>
#include <stdbool.h>

typedef struct Array Array;

Array* new_Array(size_t item_size, size_t initial_capacity);
void delete_Array(Array* a);

size_t Array_add(Array* a, void* item);
void Array_remove(Array* a, size_t index);
void* Array_get(Array* a, size_t index);
size_t Array_len(Array* a);
void  Array_clear(Array* a);

typedef void (*ArrayPrintFn)(void* item);
void Array_print(Array* a, ArrayPrintFn print);

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

typedef void (*ListPrintFn)(void* item);
void List_print(List* l, ListPrintFn print);

//========================================================

typedef struct Queue Queue;

Queue* new_Queue(size_t item_size, size_t capacity);
void delete_Queue(Queue* q);

bool Queue_push(Queue* q, void* item);
bool Queue_pop(Queue* q, void* item);

typedef void (*QueuePrintFn)(void* item);
void Queue_print(Queue* q, QueuePrintFn print);

//========================================================

typedef struct Map Map;

typedef int (*MapCompareFn)(void* key1, void* key2);
Map* new_Map(size_t key_size, size_t value_size, MapCompareFn compare);
void delete_Map(Map* m);

void Map_insert(Map* m, void* key, void* value);
void Map_remove(Map* m, void* key);
void* Map_find(Map* m, void* key);

typedef void (*MapPrintFn)(void* value);
void Map_print(Map* m, MapPrintFn print);

//========================================================

typedef struct HashMap HashMap;

typedef bool (*HashMapEqualFn)(void* key1, void* key2);
HashMap* new_HashMap(size_t key_size, size_t value_size, size_t capacity,
                HashMapEqualFn equal);
void delete_HashMap(HashMap* m);

void HashMap_insert(HashMap* m, void* key, void* value);
void HashMap_remove(HashMap* m, void* key);
void* HashMap_find(HashMap* m, void* key);

typedef void (*HashMapPrintFn)(void* key, void* value);
void HashMap_print(HashMap* m, HashMapPrintFn print);

//========================================================

typedef struct Heap Heap;

typedef bool (*HeapBiggerFn)(void* item1, void* item2);
Heap* new_Heap(size_t item_size, size_t capacity, HeapBiggerFn bigger);
void delete_Heap(Heap* h);

bool Heap_push(Heap* h, void* item);
bool Heap_pop(Heap* h, void* item);

typedef void (*HeapPrintFn)(void* item);
void Heap_print(Heap* h, HeapPrintFn print);

//========================================================

typedef struct BTree BTree;

typedef int (*BTreeCompareFn)(void* key1, void* key2);
BTree* new_BTree(int min_degree, size_t key_size, size_t value_size, BTreeCompareFn compare);
void delete_BTree(BTree* t);

void BTree_insert(BTree* t, void* key, void* value);
void BTree_remove(BTree* t, void* key);
void* BTree_find(BTree* t, void* key);

typedef void (*BTreePrintFn)(void* value);
void BTree_print(BTree* t, BTreePrintFn print);

//========================================================

typedef struct BpTree BpTree;

BpTree* new_BpTree(int order, size_t value_size);
void delete_BpTree(BpTree* t);

void BpTree_insert(BpTree* t, int key, void* value);
void BpTree_remove(BpTree* t, int key);
void* BpTree_find(BpTree* t, int key);

typedef void (*BpTreePrintFn)(void* value);
void BpTree_print(BpTree* t, BpTreePrintFn print);

//========================================================

typedef struct Graph Graph;

Graph* new_Graph(size_t item_size, size_t initial_capacity);
void delete_Graph(Graph* g);

typedef void (*GraphPrintFn)(int vertex);
void Graph_print(Graph* g, GraphPrintFn print);

int Graph_add_vertex(Graph* g, void* item);
void Graph_add_edge(Graph* g, int src, int dst, int weight);

typedef void (*GraphSearchFn)(void* item);
void Graph_dfs(Graph* g, int vertex, GraphSearchFn fn);
void Graph_bfs(Graph* g, int vertex, GraphSearchFn fn);

void Graph_mst(Graph* g);

typedef void (*GraphPathFn)(int cost, Array* path);
void Graph_path(Graph* g, int src, int dst, GraphPathFn fn);

#endif

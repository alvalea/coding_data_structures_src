#ifndef __ALGO_CONTAINER__
#define __ALGO_CONTAINER__

typedef struct Array Array;

Array* new_Array(int item_size, int initial_capacity);
void delete_Array(Array* a);

int Array_add(Array* a, void* item);
void* Array_get(Array* a, int index);
int Array_len(Array* a);
void  Array_clear(Array* a);

//========================================================

typedef struct ListNode ListNode;

void* ListNode_data(ListNode* n);
ListNode* ListNode_next(ListNode* n);

typedef struct List List;

List* new_List(int item_size);
void delete_List(List* l);

void* List_add(List* l, void* item);
void List_remove(List* l, void* item);
ListNode* List_head(List* l);
ListNode* List_tail(List* l);
ListNode* List_find(List* l, void* item);

#endif

#include <algo/container_test.h>

#include <stdio.h>
#include <algo/container.h>

typedef struct Value {
        int number;
} Value;

static
void test_List_print(void* item) {
        Value* v = (Value*)item;
        printf("%d ", v->number);
}

int test_List_add() {
        int result = 0;
        List* l = new_List(sizeof(Value));
        {
                for (int i=1; i<=20; ++i) {
                        List_add(l, &(Value){ .number = i});
                }

                List_print(l, test_List_print);
        }
        delete_List(l);
        return result;
}

int test_List_remove() {
        int result = 0;
        List* l = new_List(sizeof(int));
        {
                int* x = List_add(l, &(int){3});
                List_remove(l, x);
                if (List_head(l) != NULL) {
                        result = -1;
                }
        }
        delete_List(l);
        return result;
}

int test_List_head() {
        int result = 0;
        List* l = new_List(sizeof(int));
        {
                int* x = List_add(l, &(int){3});
                ListNode* head = List_head(l);
                if (ListNode_data(head) != x) {
                        result = -1;
                }
        }
        delete_List(l);
        return result;
}

int test_List_tail() {
        int result = 0;
        List* l = new_List(sizeof(int));
        {
                int* x = List_add(l, &(int){3});
                ListNode* tail = List_tail(l);
                if (ListNode_data(tail) != x) {
                        result = -1;
                }
        }
        delete_List(l);
        return result;
}

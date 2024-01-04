#include <algo/container_test.h>

#include <stdio.h>
#include <stdlib.h>
#include <algo/container.h>

#define STR 10

typedef char string[STR];

typedef struct Value {
        int number;
} Value;

static
void test_Heap_print(void* item) {
        Value* v = (Value*)item;
        printf("%d ", v->number);
}

static
bool test_Heap_bigger_int(void* item1, void* item2) {
        Value* v1 = (Value*)item1;
        Value* v2 = (Value*)item2;
        return v1->number > v2->number;
}

static
bool test_Heap_bigger_string(void* item1, void* item2) {
        int i1 = atoi(item1);
        int i2 = atoi(item2);
        return i1 > i2;
}

int test_Heap_push() {
        int result = 0;
        int n = 9;
        Heap* h = new_Heap(sizeof(int), n, test_Heap_bigger_int);
        {
                printf("\n");
                for (int i = 1; i<=n; ++i) {
                        Heap_push(h, &(Value){ .number = i });
                }
                Heap_print(h, test_Heap_print);
                //               9
                //              / \
                //             8   6
                //            / \  /\
                //          7   3 2  5
                //         /\
                //        1 4

                printf("\n");
                for (int i = n/2; i>0; --i) {
                        Value v;
                        Heap_pop(h, &v);
                }
                Heap_print(h, test_Heap_print);
                //              5
                //             /\
                //            4 2
                //           /\
                //          1 3
                printf("\n");
        }
        delete_Heap(h);
        return result;
}

int test_Heap_pop() {
        int result = 0;
        string str;
        int n = 10;
        Heap* h = new_Heap(sizeof(string), n, test_Heap_bigger_string);
        {
                for (int i = 1; i<=n; ++i) {
                        sprintf(str, "%d", i);
                        Heap_push(h, str);
                }
                for (int i = n; i>0; --i) {
                        Heap_pop(h, str);
                        int x = atoi(str);
                        if (x != i) {
                                result = -1;
                        }
                }
        }
        delete_Heap(h);
        return result;
}


#include <algo/container_test.h>

#include <stdio.h>
#include <algo/container.h>

#define STR 10
typedef char string[STR];

typedef struct Value {
        int number;
        string text;
} Value;

static
void test_BpTree_print(void* value) {
        Value* v = (Value*)value;
        printf("[%s] ",v->text);
}

int test_BpTree_insert() {
        int result = 0;
        for (int i=3; i<4; ++i) {
                BpTree* t = new_BpTree(i, sizeof(Value));
                {
                        for (int j=11; j>0; --j) {
				Value v = { .number = j };
				sprintf(v.text, "%d", j*2);
                                BpTree_insert(t, j, &v);
                        }

                        printf("\n");
                        BpTree_print(t, test_BpTree_print);

                        for (int j = 11; j > 0; --j) {
                                if (j%3== 0) {
                                        BpTree_remove(t, j);
                                }
                        }

                        printf("\n");
                        BpTree_print(t, test_BpTree_print);
                }
                delete_BpTree(t);
        }
        return result;
}

int test_BpTree_find() {
        int result = 0;
        for (int i=3; i<6; ++i) {
                BpTree* t = new_BpTree(i, sizeof(int));
                {
                        for (int j=1; j<=50; ++j) {
                                BpTree_insert(t, j, &(int){j});
                        }
                        for (int j=1; j<=50; ++j) {
                                if (j%3== 0) {
                                        BpTree_remove(t, j);
                                }
                        }
                }

                int* x = (int*)BpTree_find(t, 20);
                if (*x != 20) {
                        result = -1;
                }
                delete_BpTree(t);
        }
        return result;

}

#include <algo/container_test.h>

#include <stdio.h>
#include <algo/container.h>

typedef struct Value {
        int number;
} Value;

static
void test_Array_print(void* item) {
        Value* v = (Value*)item;
        printf("%d ", v->number);
}

int test_Array_add() {
        int result = 0;
        Array* a = new_Array(sizeof(Value), 10);
        {
                for (int i=1; i<=20; ++i) {
                        Array_add(a, &(Value){ .number = i});
                }

                int len = Array_len(a);
                for (int i=0; i<len; ++i) {
                        Value* v = Array_get(a, i);
                        if (v != NULL && v->number % 2) {
                                Array_remove(a, i);
                        }
                }

                Array_print(a, test_Array_print);
        }
        delete_Array(a);
        return result;
}

int test_Array_remove() {
        int result = 0;
        Array* a = new_Array(sizeof(int), 10);
        {
                for (int i=1; i<=20; ++i) {
                        Array_add(a, &i);
                }

                Array_remove(a, 3);
                Array_remove(a, 9);
                Array_remove(a, 15);

                if (Array_len(a) != 17) {
                        result = 1;
                }
        }
        delete_Array(a);
        return result;
}

int test_Array_get() {
        int result = 0;
        Array* a = new_Array(sizeof(int), 10);
        {
                int i = 3;
                Array_add(a, &i);
                int* x = Array_get(a, 0);
                if (*x != i) {
                        result = -1;
                }
        }
        delete_Array(a);
        return result;
}

int test_Array_len() {
        int result = 0;
        Array* a = new_Array(sizeof(int), 10);
        {
                Array_add(a, &(int){3});
                if (Array_len(a) != 1) {
                        result = -1;
                }
        }
        delete_Array(a);
        return result;
}

int test_Array_clear() {
        int result = 0;
        Array* a = new_Array(sizeof(int), 10);
        {
                Array_add(a, &(int){3});
                Array_clear(a);
                if (Array_len(a) != 0) {
                        result = -1;
                }
        }
        delete_Array(a);
        return result;
}

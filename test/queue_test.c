#include <algo/container_test.h>

#include <stdio.h>
#include <algo/container.h>

typedef struct Value {
        int number;
} Value;

static
void test_Queue_print(void* item) {
        Value* v = (Value*)item;
        printf("%d ", v->number);
}


int test_Queue_push() {
        int result = 0;
        Queue* q = new_Queue(sizeof(int), 1);
        {
                int i = 3;
                Queue_push(q, &i);
                int x;
                Queue_pop(q, &x);
                if (x != i) {
                        result = -1;
                }
        }
        delete_Queue(q);
        return result;
}

int test_Queue_pop() {
        int result = 0;
        int n = 10;
        Queue* q = new_Queue(sizeof(Value), n);
        {
                for (int i = 0; i<n; ++i) {
                        Queue_push(q, &i);
                }
                for (int i = 0; i<n/2; ++i) {
                        Value v;
                        Queue_pop(q, &v);
                }
                Queue_print(q, test_Queue_print);
        }
        delete_Queue(q);
        return result;
}

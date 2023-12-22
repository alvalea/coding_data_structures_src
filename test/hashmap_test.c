#include <algo/container_test.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algo/container.h>

#define STR 8
typedef char string[STR];

typedef struct Key {
        string str;
} Key;

typedef struct Value {
        int number;
} Value;

static
const size_t CAPACITY = 6;

static
bool test_HashMap_equal(void* key1, void* key2) {
        int* k1 = (int*)key1;
        int* k2 = (int*)key2;
        return *k1 == *k2;
}

static
bool test_HashMap_equal_kv(void* key1, void* key2) {
        Key* k1 = (Key*)key1;
        Key* k2 = (Key*)key2;
        return strcmp(k1->str, k2->str) == 0;
}

static
void tets_HashMap_print_kv(void* key, void* value) {
        Key* k = (Key*)key;
        printf("[%s|", k->str);
        Value* v = (Value*)value;
        printf("%d]", v->number);
}

int test_HashMap_insert() {
        int result = 0;
        HashMap* m = new_HashMap(sizeof(Key), sizeof(Value), CAPACITY,
                        test_HashMap_equal_kv);
        {
                int n = 6;
                string keys[] = { "Alice", "Celia", "Andrew", "Darwin",
                        "Adeline", "Delaine" };
                for (int i=0; i<n; ++i) {
                        Key k;
                        strcpy(k.str, keys[i]);
                        HashMap_insert(m, &k, &(Value){.number=i});
                }
                HashMap_remove(m, &(Key){.str = "Celia"});
                printf("\n");
                HashMap_print(m, tets_HashMap_print_kv);
        }
        delete_HashMap(m);
        return result;
}

int test_HashMap_remove() {
        int result = 0;
        HashMap* m = new_HashMap(sizeof(int), sizeof(int), CAPACITY,
                        test_HashMap_equal);
        {
                int i = 3;
                HashMap_insert(m, &i, &i);
                HashMap_remove(m, &i);
                int* x = HashMap_find(m, &i);
                if (x != NULL){
                        result = -1;
                }
        }
        delete_HashMap(m);
        return result;
}

int test_HashMap_find() {
        int result = 0;
        HashMap* m = new_HashMap(sizeof(int), sizeof(int), CAPACITY,
                        test_HashMap_equal);
        {
                for (int i=0; i<100; ++i) {
                        HashMap_insert(m, &i, &i);
                }
                int i = 99;
                int* x = HashMap_find(m, &i);
                if (*x != i){
                        result = -1;
                }
        }
        delete_HashMap(m);
        return result;
}

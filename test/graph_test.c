#include <algo/container_test.h>

#include <stdio.h>
#include <algo/container.h>

typedef struct Value {
        int number;
} Value;

static
void test_print_int(int vertex) {
        printf("%d ", vertex);
}

int test_Graph_print() {
        int result = 0;
        Graph* g = new_Graph(sizeof(Value), 10);
        {
                for (int i=0; i<4; ++i) {
                        Graph_add_node(g, &(Value){.number=i*2});
                }

                Graph_add_edge(g, 0, 1, 1);
                Graph_add_edge(g, 1, 0, 1);

                Graph_add_edge(g, 0, 2, 1);
                Graph_add_edge(g, 2, 0, 1);

                Graph_add_edge(g, 1, 2, 1);
                Graph_add_edge(g, 2, 1, 1);

                Graph_add_edge(g, 2, 3, 1);
                Graph_add_edge(g, 3, 2, 1);

                Graph_print(g, test_print_int);
        }
        delete_Graph(g);
        return result;
}

static
void test_print_item(void* item) {
        Value* v = (Value*)item;
        printf("%d ", v->number);
}

typedef void (*TestGraphFn)(Graph*, int, GraphCallbackFn);

static
int test_Graph_search(TestGraphFn fn) {
        int result = 0;
        Graph* g = new_Graph(sizeof(Value), 10);
        {
                for (int i=0; i<4; ++i) {
                        Graph_add_node(g, &(Value){.number=i*2});
                }

                Graph_add_edge(g, 0, 1, 1);
                Graph_add_edge(g, 1, 0, 1);

                Graph_add_edge(g, 0, 2, 1);
                Graph_add_edge(g, 2, 0, 1);

                Graph_add_edge(g, 1, 2, 1);
                Graph_add_edge(g, 2, 1, 1);

                Graph_add_edge(g, 2, 3, 1);
                Graph_add_edge(g, 3, 2, 1);

                fn(g, 2, test_print_item);
        }
        delete_Graph(g);
        return result;
}

int test_Graph_dfs() {
        int result = test_Graph_search(Graph_dfs);
        return result;
}

int test_Graph_bfs() {
        int result = test_Graph_search(Graph_bfs);
        return result;
}

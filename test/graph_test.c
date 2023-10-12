#include <algo/container_test.h>

#include <stdio.h>
#include <algo/container.h>

typedef struct Value {
        int number;
        int number1;
        int number2;
        int number3;
        int number4;
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

                //Graph_print(g, test_print_int);
        }
        delete_Graph(g);
        return result;
}

static
void test_print_item(void* item) {
        Value* v = (Value*)item;
        printf("%d ", v->number);
}

typedef void (*TestGraphFn)(Graph*, int, GraphSearchFn);

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

                //fn(g, 2, test_print_item);
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

int test_Graph_mst() {
        int result = 0;
        Graph* g = new_Graph(sizeof(Value), 10);
        {
                for (int i=0; i<5; ++i) {
                        Graph_add_node(g, &(Value){.number=i*2});
                }

                Graph_add_edge(g, 0, 1, 2);
                Graph_add_edge(g, 0, 3, 6);

                Graph_add_edge(g, 1, 0, 2);
                Graph_add_edge(g, 1, 2, 3);
                Graph_add_edge(g, 1, 3, 8);
                Graph_add_edge(g, 1, 4, 5);

                Graph_add_edge(g, 2, 1, 3);
                Graph_add_edge(g, 2, 4, 7);

                Graph_add_edge(g, 3, 0, 6);
                Graph_add_edge(g, 3, 1, 8);
                Graph_add_edge(g, 3, 4, 9);

                Graph_add_edge(g, 4, 1, 5);
                Graph_add_edge(g, 4, 2, 7);
                Graph_add_edge(g, 4, 3, 9);

                //Graph_print(g, test_print_int);

                Graph_mst(g);

                //Graph_print(g, test_print_int);
        }
        delete_Graph(g);
        return result;
}

static
void test_graph_shortest_path(int weight, Array* path) {
        printf("weight: %d\n", weight);
        size_t len = Array_len(path);
        for (int i=0; i<len; ++i) {
                Value* v = *(Value**)Array_get(path, i);
                printf("%d -> ", v->number4);
        }
        printf("\n");
}

int test_Graph_path() {
        int result = 0;
        Graph* g = new_Graph(sizeof(Value), 10);
        {
                for (int i=0; i<5; ++i) {
                        Graph_add_node(g, 
                                        &(Value){
                                        .number=i,
                                        .number1=i,
                                        .number2=i,
                                        .number3=i,
                                        .number4=i,
                                        });
                }

                Graph_add_edge(g, 0, 1, 2);
                Graph_add_edge(g, 0, 3, 6);

                Graph_add_edge(g, 1, 0, 2);
                Graph_add_edge(g, 1, 2, 3);
                Graph_add_edge(g, 1, 3, 8);
                Graph_add_edge(g, 1, 4, 5);

                Graph_add_edge(g, 2, 1, 3);
                Graph_add_edge(g, 2, 4, 7);

                Graph_add_edge(g, 3, 0, 6);
                Graph_add_edge(g, 3, 1, 8);
                Graph_add_edge(g, 3, 4, 9);

                Graph_add_edge(g, 4, 1, 5);
                Graph_add_edge(g, 4, 2, 7);
                Graph_add_edge(g, 4, 3, 9);

                //printf("\n");
                //printf("src: 0 dst: 2 ");
                //Graph_path(g, 0, 2, test_graph_shortest_path);
        }
        delete_Graph(g);
        return result;
}

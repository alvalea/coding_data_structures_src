#include <algo/container_test.h>

#include <stdio.h>
#include <algo/container.h>

typedef struct Value {
        int number;
} Value;

void test_Graph_print(void* item) {
        int* v = (int*)item;
        printf("%d ", *v);
}

void test_Graph_dfs_print(void* item) {
        Value* v = (Value*)item;
        printf("%d ", v->number);
}

int test_Graph_add() {
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

                Graph_print(g, test_Graph_print);

                Graph_dfs(g, 2, test_Graph_dfs_print);

                printf("\n");
        }
        delete_Graph(g);
        return result;
}

#include <algo/container_test.h>

#include <stdio.h>
#include <algo/container.h>

void test_Graph_print(void* item) {
        int* v = (int*)item;
        printf("%d ", *v);
}

int test_Graph_add() {
        int result = 0;
        Graph* g = new_Graph(10);
        {
                Graph_add(g, 0, 1);
                Graph_add(g, 0, 2);
                Graph_add(g, 1, 2);
                Graph_add(g, 2, 3);

                Graph_print(g, test_Graph_print);

                Graph_dfs(g, 2);
        }
        delete_Graph(g);
        return result;
}

#include <algo/container.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Graph {
        int vertices;
        Array* visited;
        Array** nodes;
};

Graph* new_Graph(int vertices) {
        Graph* g = calloc(1, sizeof(Graph));

        g->vertices = vertices;
        g->visited = new_Array(sizeof(bool), vertices);
        for (int i=0; i<g->vertices; ++i) {
                Array_add(g->visited, &(bool){false});
        }

        g->nodes = calloc(vertices, sizeof(Array*));
        for (int i=0; i<g->vertices; ++i) {
                g->nodes[i] = new_Array(sizeof(int), vertices);
        }

        return g;
}

void delete_Graph(Graph* g) {
        delete_Array(g->visited);
        for (int i=0; i<g->vertices; ++i) {
                delete_Array(g->nodes[i]);
        }
        free(g->nodes);
        free(g);
}

void Graph_add(Graph* g, int src, int dst) {
        Array_add(g->nodes[src], &dst);
        Array_add(g->nodes[dst], &src);
}

void Graph_dfs(Graph* g, int vertex) {
        bool* visited = (bool*)Array_get(g->visited, vertex);
        *visited = true;
        printf("Visited %d \n", vertex);

        Array* nodes = g->nodes[vertex];
        size_t len = Array_len(nodes);
        if (len > 0) {
                for (size_t j=0; j<len; ++j) {
                        int* connectedVertex = (int*)Array_get(nodes, j);
                        bool* conn_visited = (bool*)Array_get(g->visited, *connectedVertex);
                        if (!*conn_visited) {
                                Graph_dfs(g, *connectedVertex);
                        }
                }
        }
}

void Graph_print(Graph* g, GraphPrintFn print) {
        printf("\n");
        for (int i = 0; i < g->vertices; ++i) {
                Array* nodes = g->nodes[i];
                size_t len = Array_len(nodes);
                if (len > 0) {
                        for (size_t j=0; j<len; ++j) {
                                print(Array_get(nodes, j));
                        }
                        printf("\n");
                }
        }
}

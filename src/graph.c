#include <algo/container.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Edge {
        int weight;
        int vertex;
} Edge;

typedef struct Vertex {
        bool visited;
        Array* edges;
} Vertex;

struct Graph {
        Array* vertices;
};

Graph* new_Graph(int vertices) {
        Graph* g = calloc(1, sizeof(Graph));
        
        g->vertices = new_Array(sizeof(Vertex), vertices);
        for (int i=0; i<vertices; ++i) {
                Vertex vertex = {
                        .visited = false,
                        .edges = new_Array(sizeof(Edge), vertices)
                };
                Array_add(g->vertices, &vertex);
        }

        return g;
}

void delete_Graph(Graph* g) {
        size_t len = Array_len(g->vertices);
        for (int i=0; i<len; ++i) {
                Vertex* vertex = Array_get(g->vertices, i);
                delete_Array(vertex->edges);
        }
        delete_Array(g->vertices);
        free(g);
}

void Graph_add(Graph* g, int src, int dst) {
        Vertex* src_vertex = Array_get(g->vertices, src);
        Edge src_edge = {.weight=1, .vertex=dst};
        Array_add(src_vertex->edges, &src_edge);

        Vertex* dst_vertex = Array_get(g->vertices, dst);
        Edge dst_edge = {.weight=1, .vertex=src};
        Array_add(dst_vertex->edges, &dst_edge);
}

void Graph_dfs(Graph* g, int i) {
        Vertex* vertex = Array_get(g->vertices, i);
        vertex->visited = true;
        printf("Visited %d \n", i);

        size_t len = Array_len(vertex->edges);
        for (size_t j=0; j<len; ++j) {
                Edge* edge = (Edge*)Array_get(vertex->edges, j);
                Vertex* conn_vertex = (Vertex*)Array_get(g->vertices, edge->vertex);
                if (!conn_vertex->visited) {
                        Graph_dfs(g, edge->vertex);
                }
        }
}

void Graph_print(Graph* g, GraphPrintFn print) {
        printf("\n");
        size_t len_vertices = Array_len(g->vertices);
        for (int i=0; i<len_vertices; ++i) {
                Vertex* vertex = Array_get(g->vertices, i);
                size_t len_edges = Array_len(vertex->edges);
                for (int j=0; j<len_edges; ++j) {
                        Edge* edge = (Edge*)Array_get(vertex->edges, j);
                        print(&edge->vertex);
                }
                if (len_edges > 0) {
                        printf("\n");
                }
        }
}

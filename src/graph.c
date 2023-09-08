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
        void* data;
        Array* edges;
} Vertex;

struct Graph {
        size_t item_size;
        size_t initial_capacity;
        Array* vertices;
};

Graph* new_Graph(size_t item_size, size_t initial_capacity) {
        Graph* g = calloc(1, sizeof(Graph));
        
        g->item_size = item_size;
        g->initial_capacity = initial_capacity;
        g->vertices = new_Array(sizeof(Vertex), initial_capacity);

        return g;
}

void delete_Graph(Graph* g) {
        size_t len = Array_len(g->vertices);
        for (int i=0; i<len; ++i) {
                Vertex* vertex = Array_get(g->vertices, i);
                free(vertex->data);
                delete_Array(vertex->edges);
        }
        delete_Array(g->vertices);
        free(g);
}

int Graph_add_node(Graph* g, void* item) {
        size_t len = Array_len(g->vertices);
        if (len < g->initial_capacity) {
                len = g->initial_capacity;
        }
        Vertex vertex = {
                .visited = false,
                .edges = new_Array(sizeof(Edge), len)
        };
        vertex.data = malloc(g->item_size);
        memcpy(vertex.data, item, g->item_size);
        Array_add(g->vertices, &vertex);

        return Array_len(g->vertices);
}

void Graph_add_edge(Graph* g, int src, int dst, int weight) {
        Vertex* src_vertex = Array_get(g->vertices, src);
        Edge src_edge = {.weight=weight, .vertex=dst};
        Array_add(src_vertex->edges, &src_edge);
}

void Graph_dfs(Graph* g, int i, GraphPrintFn print) {
        Vertex* vertex = Array_get(g->vertices, i);
        vertex->visited = true;
        print(vertex->data);

        size_t len = Array_len(vertex->edges);
        for (size_t j=0; j<len; ++j) {
                Edge* edge = (Edge*)Array_get(vertex->edges, j);
                Vertex* conn_vertex = (Vertex*)Array_get(g->vertices, edge->vertex);
                if (!conn_vertex->visited) {
                        Graph_dfs(g, edge->vertex, print);
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

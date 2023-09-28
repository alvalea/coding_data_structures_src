#include <algo/container.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Edge {
        bool selected;
        int weight;
        int vertex;
} Edge;

typedef struct Vertex {
        bool selected;
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
                .selected = false,
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

static
void Graph_clean(Graph* g) {
        size_t len = Array_len(g->vertices);
        for (int i=0; i<len; ++i) {
                Vertex* v = Array_get(g->vertices, i);
                v->selected = false;

                size_t len = Array_len(v->edges);
                for (int j=0; j<len; ++j) {
                        Edge* edge = Array_get(v->edges, j);
                        edge->selected = false;
                }
        }
}

static
void Graph_dfs_loop(Graph* g, int vertex, GraphCallbackFn callback) {
        Vertex* v = Array_get(g->vertices, vertex);
        v->selected = true;
        callback(v->data);

        size_t len = Array_len(v->edges);
        for (int i=0; i<len; ++i) {
                Edge* edge = Array_get(v->edges, i);
                Vertex* conn_vertex = Array_get(g->vertices, edge->vertex);
                if (!conn_vertex->selected) {
                        Graph_dfs_loop(g, edge->vertex, callback);
                }
        }
}

// Input:
//        A
//       / \
//      B   C
//     /   / \
//    D   E   F
// Output:
// A, B, D, C, E, F
void Graph_dfs(Graph* g, int vertex, GraphCallbackFn callback) {
        Graph_clean(g);

        Graph_dfs_loop(g, vertex, callback);
}

static
void Graph_bfs_loop(Graph* g, Queue* queue, GraphCallbackFn callback) {
        int current_vertex = 0;
        while(Queue_pop(queue, &current_vertex)) {
                Vertex* v = Array_get(g->vertices, current_vertex);
                callback(v->data);

                size_t len = Array_len(v->edges);
                for (int i=0; i<len; ++i) {
                        Edge* edge = Array_get(v->edges, i);
                        Vertex* conn_vertex = Array_get(g->vertices, edge->vertex);
                        if (!conn_vertex->selected) {
                                conn_vertex->selected = true;
                                Queue_push(queue, &edge->vertex);
                        }
                }
        }
}

// Input:
//        A
//       / \
//      B   C
//     /   / \
//    D   E   F
// Output:
// A, B, C, D, E, F
void Graph_bfs(Graph* g, int vertex, GraphCallbackFn callback) {
        Graph_clean(g);

        Queue* queue = new_Queue(sizeof(int), g->initial_capacity);
        {
                Vertex* v = Array_get(g->vertices, vertex);
                v->selected = true;
                Queue_push(queue, &vertex);
                Graph_bfs_loop(g, queue, callback);
        }
        delete_Queue(queue);
}

void Graph_print(Graph* g, GraphPrintFn print) {
        printf("\n");
        size_t len_vertices = Array_len(g->vertices);
        for (int i=0; i<len_vertices; ++i) {
                Vertex* vertex = Array_get(g->vertices, i);

                for (int j=0, e=0; j<len_vertices; ++j) {
                        Edge* edge = Array_get(vertex->edges, e);
                        if (edge && (edge->vertex == j)) {
                                print(edge->weight);
                                ++e;
                        } else {
                                print(0);
                        }
                }
                printf("\n");
        }
}

static
void Graph_mst_prune(Graph* g) {
        size_t len = Array_len(g->vertices);
        for (int i=0; i<len; ++i) {
                Vertex* vertex = Array_get(g->vertices, i);
                size_t len_edges = Array_len(vertex->edges);
                for (int j=len_edges-1; j>=0; --j) {
                        Edge* edge = Array_get(vertex->edges, j);
                        if (edge && !edge->selected) {
                                Array_remove(vertex->edges, j);
                        }
                }
        }
}

typedef struct SelectedEdge {
        int vertex;
        int edge_index;
} SelectedEdge;

static
SelectedEdge Graph_mst_min_weight(Graph* g) {
        SelectedEdge selected_edge;

        int min = INT_MAX;
        size_t len = Array_len(g->vertices);
        for (int i=0; i<len; ++i) {
                Vertex* src_vertex = Array_get(g->vertices, i);
                if (src_vertex->selected) {
                        size_t len_edges = Array_len(src_vertex->edges);
                        for (int j=0; j<len_edges; ++j) {
                                Edge* edge = Array_get(src_vertex->edges, j);
                                if (edge != NULL) {
                                        Vertex* dst_vertex = Array_get(g->vertices, edge->vertex);
                                        if (!dst_vertex->selected && min > edge->weight) {
                                                min = edge->weight;
                                                selected_edge.vertex = i;
                                                selected_edge.edge_index = j;
                                        }
                                }
                        }
                }
        }
        return selected_edge;
}

void Graph_mst(Graph* g) {
        Graph_clean(g);

        Vertex* vertex = Array_get(g->vertices, 0);
        vertex->selected = true;

        size_t len = Array_len(g->vertices);
        for (int v=0; v<len-1; ++v) {
                SelectedEdge selected_edge = Graph_mst_min_weight(g);

                vertex = Array_get(g->vertices, selected_edge.vertex);

                Edge* edge = Array_get(vertex->edges, selected_edge.edge_index);
                edge->selected = true;
                vertex = Array_get(g->vertices, edge->vertex);
                vertex->selected = true;
        }

        Graph_mst_prune(g);
}

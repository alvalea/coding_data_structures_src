#include <algo/container.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

typedef struct Edge {
        bool selected;
        int weight;
        int src;
        int dst;
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

int Graph_add_vertex(Graph* g, void* item) {
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
        Edge src_edge = {.weight=weight, .src=src, .dst=dst};
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
void Graph_dfs_loop(Graph* g, int vertex, GraphSearchFn fn) {
        Vertex* v = Array_get(g->vertices, vertex);
        v->selected = true;
        fn(v->data);

        size_t len = Array_len(v->edges);
        for (int i=0; i<len; ++i) {
                Edge* edge = Array_get(v->edges, i);
                Vertex* conn_vertex = Array_get(g->vertices, edge->dst);
                if (!conn_vertex->selected) {
                        Graph_dfs_loop(g, edge->dst, fn);
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
void Graph_dfs(Graph* g, int vertex, GraphSearchFn fn) {
        Graph_clean(g);

        Graph_dfs_loop(g, vertex, fn);
}

static
void Graph_bfs_loop(Graph* g, Queue* queue, GraphSearchFn fn) {
        int current_vertex = 0;
        while(Queue_pop(queue, &current_vertex)) {
                Vertex* v = Array_get(g->vertices, current_vertex);
                fn(v->data);

                size_t len = Array_len(v->edges);
                for (int i=0; i<len; ++i) {
                        Edge* edge = Array_get(v->edges, i);
                        Vertex* conn_vertex = Array_get(g->vertices, edge->dst);
                        if (!conn_vertex->selected) {
                                conn_vertex->selected = true;
                                Queue_push(queue, &edge->dst);
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
void Graph_bfs(Graph* g, int vertex, GraphSearchFn fn) {
        Graph_clean(g);

        Queue* queue = new_Queue(sizeof(int), g->initial_capacity);
        {
                Vertex* v = Array_get(g->vertices, vertex);
                v->selected = true;
                Queue_push(queue, &vertex);
                Graph_bfs_loop(g, queue, fn);
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
                        if (edge && (edge->dst == j)) {
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
void Graph_mst_select_edges(Graph* g, Array* mst) {
        size_t len = Array_len(mst);
        for (int i=1; i<len; ++i) {
                Edge* selected_edge = Array_get(mst, i);

                Vertex* vertex = Array_get(g->vertices, selected_edge->src);
                size_t len_edges = Array_len(vertex->edges);
                for (int j=0; j<len_edges; ++j) {
                        Edge* edge = Array_get(vertex->edges, j);
                        if (edge->dst == selected_edge->dst) {
                                edge->selected = true;
                        }
                }
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

static
Edge* Graph_min_edge(Graph* g, Array* mst) {
        int min_index = -1;
        int min = INT_MAX;
        size_t len = Array_len(g->vertices);
        for (int i=0; i<len; ++i) {
                Edge* edge = Array_get(mst, i);
                if (!edge->selected && edge->weight < min) {
                        min = edge->weight;
                        min_index = edge->dst;
                }
        }
        Edge* edge = Array_get(mst, min_index);
        edge->selected = true;

        return edge;
}

static
void Graph_int_edges(Graph* g, Array* edges, int index) {
        size_t len = Array_len(g->vertices);
        for (int i=0; i<len; ++i) {
                Array_add(edges, &(Edge){.weight=INT_MAX});
        }
        Edge* e = Array_get(edges, index);
        e->weight = 0;
        e->src = -1;
}

static
void Graph_mst_update_edges(Graph* g, Array* mst, Edge* edge) {
        Vertex* vertex = Array_get(g->vertices, edge->dst);

        size_t len = Array_len(g->vertices);
        for (int j=0, v=0; v<len; ++v) {
                Edge* mst_edge = Array_get(mst, v);
                Edge* vertex_edge = Array_get(vertex->edges, j);
                if ( vertex_edge && vertex_edge->dst == v) {
                        if (!mst_edge->selected &&
                            mst_edge->weight > vertex_edge->weight) {
                                mst_edge->src = vertex_edge->src;
                                mst_edge->dst = vertex_edge->dst;
                                mst_edge->weight = vertex_edge->weight;
                        }
                        ++j;
                }
        }
}

void Graph_mst(Graph* g) {
        Graph_clean(g);

        size_t len = Array_len(g->vertices);
        Array* mst = new_Array(sizeof(Edge), len);
        {
                Graph_int_edges(g, mst, 0);

                for (int i=0; i<len-1; ++i) {
                        Edge* edge = Graph_min_edge(g, mst);
                        Graph_mst_update_edges(g, mst, edge);
                }
                Graph_mst_select_edges(g, mst);
                Graph_mst_prune(g);
        }
        delete_Array(mst);
}

static
void Graph_path_vertices(Graph* g, int vertex, Array* path, Array* vertices) {
        if (vertex < 0){
                return;
        }
        Edge* edge = Array_get(path, vertex);
        Graph_path_vertices(g, edge->src, path, vertices);

        Vertex* v = Array_get(g->vertices, vertex);
        Array_add(vertices, &v->data);
}

static
int Graph_path_cost(Graph* g, Array* path, int dst) {
        Edge* edge = Array_get(path, dst);
        Vertex* vertex = Array_get(g->vertices, edge->dst);
        return edge->weight;
}

static
void Graph_path_result(Graph* g, GraphPathFn fn, Array* path, int dst) {
        int cost = Graph_path_cost(g, path, dst);
        size_t len = Array_len(g->vertices);
        Array* vertices = new_Array(sizeof(void*), len);
        {
                Graph_path_vertices(g, dst, path, vertices);
                fn(cost, vertices);
        }
        delete_Array(vertices);
}

static
void Graph_path_update_edges(Graph* g, Array* path, Edge* edge) {
        Vertex* vertex = Array_get(g->vertices, edge->dst);

        size_t len = Array_len(g->vertices);
        for (int j=0, v=0; v<len; ++v) {
                Edge* path_edge = Array_get(path, v);
                Edge* vertex_edge = Array_get(vertex->edges, j);
                if ( vertex_edge && vertex_edge->dst == v) {
                        if (!path_edge->selected && edge->weight != INT_MAX &&
                                        path_edge->weight > vertex_edge->weight + edge->weight) {
                                path_edge->src = vertex_edge->src;
                                path_edge->dst = vertex_edge->dst;
                                path_edge->weight = vertex_edge->weight + edge->weight;
                        }
                        ++j;
                }
        }
}

void Graph_path(Graph* g, int src, int dst, GraphPathFn fn) {
        Graph_clean(g);

        size_t len = Array_len(g->vertices);
        Array* path = new_Array(sizeof(Edge), len);
        {
                Graph_int_edges(g, path, src);

                for (int i=0; i<len-1; ++i) {
                        Edge* edge = Graph_min_edge(g, path);
                        Graph_path_update_edges(g, path, edge);
                }
                Graph_path_result(g, fn, path, dst);
        }
        delete_Array(path);
}


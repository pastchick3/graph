// A C language wrapper of `graph.cpp`.
//
// The user should work exclusively with a `GraphPtr` obtained from
// `graph_ctor()` and is responible to call `graph_dtor()`. Also, the
// user should call `graph_free_vertices()` and `graph_free_edges()`
// when necessary.

#ifndef __PASTCHICK3_GRAPH_WRAPPER__
#define __PASTCHICK3_GRAPH_WRAPPER__

#include "graph.hpp"

#ifdef __cplusplus
extern "C" {
#endif

#include <cstddef>
#include <stdexcept>

typedef Graph<size_t, int> _Graph;

typedef void *GraphPtr;

typedef struct Edge {
    size_t to;
    int weight;
} Edge;

typedef struct Edges {
    Edge *array;
    size_t size;
} Edges;

typedef struct Vertices {
    size_t *array;
    size_t size;
} Vertices;

_Graph *_cast_graph_ptr(GraphPtr graph_ptr) {
    return reinterpret_cast<_Graph *>(graph_ptr);
}

GraphPtr graph_ctor() { return reinterpret_cast<GraphPtr>(new _Graph{}); }

void graph_dtor(GraphPtr graph_ptr) {
    auto graph = _cast_graph_ptr(graph_ptr);
    delete graph;
}

void graph_insert_vertex(GraphPtr graph_ptr, size_t vertex) {
    auto graph = _cast_graph_ptr(graph_ptr);
    graph->insert_vertex(vertex);
}

void graph_insert_edge(GraphPtr graph_ptr, size_t from, size_t to, int weight) {
    auto graph = _cast_graph_ptr(graph_ptr);
    try {
        graph->insert_edge(from, to, weight);
    } catch (std::runtime_error) {
    }
}

void graph_remove_vertex(GraphPtr graph_ptr, size_t vertex) {
    auto graph = _cast_graph_ptr(graph_ptr);
    graph->remove_vertex(vertex);
}

void graph_remove_edge(GraphPtr graph_ptr, size_t from, size_t to) {
    auto graph = _cast_graph_ptr(graph_ptr);
    graph->remove_edge(from, to);
}

size_t graph_get_vertex_number(GraphPtr graph_ptr) {
    auto graph = _cast_graph_ptr(graph_ptr);
    return graph->get_vertex_number();
}

size_t graph_get_edge_number(GraphPtr graph_ptr) {
    auto graph = _cast_graph_ptr(graph_ptr);
    return graph->get_edge_number();
}

const Vertices *graph_get_vertices(GraphPtr graph_ptr) {
    auto graph = _cast_graph_ptr(graph_ptr);
    auto vec = graph->get_vertices();
    auto vertices =
        new const Vertices{.array = new size_t[vec.size()], .size = vec.size()};
    for (auto i = 0; i < vec.size(); ++i) {
        vertices->array[i] = vec[i];
    }
    return vertices;
}

void graph_free_vertices(const Vertices *vertices) {
    delete[] vertices->array;
    delete vertices;
}

const Edges *graph_get_adjacent_edges(GraphPtr graph_ptr, size_t vertex) {
    auto graph = _cast_graph_ptr(graph_ptr);
    auto vec = graph->get_adjacent_edges(vertex);
    auto edges =
        new const Edges{.array = new Edge[vec.size()], .size = vec.size()};
    for (auto i = 0; i < vec.size(); ++i) {
        edges->array[i] = Edge{.to = vec[i].first, .weight = vec[i].second};
    }
    return edges;
}

void graph_free_edges(const Edges *edges) {
    delete[] edges->array;
    delete edges;
}

#ifdef __cplusplus
}
#endif

#endif

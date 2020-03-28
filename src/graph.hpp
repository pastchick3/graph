#ifndef __PASTCHICK3_GRAPH__
#define __PASTCHICK3_GRAPH__

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

// A basic graph implementation based on the adjacent list.
template <typename Vertex = std::string, typename Weight = double>
class Graph {
   public:
    // Each `Edge` contains the target vertex and its weight.
    typedef std::pair<Vertex, Weight> Edge;

    Graph();
    Graph(std::initializer_list<Vertex> vertices,
          std::initializer_list<std::pair<Vertex, Edge>> edges);
    Graph(const Graph &graph);
    Graph(Graph &&graph);
    Graph &operator=(const Graph &graph);
    Graph &operator=(Graph &&graph);
    ~Graph();

    void insert_vertex(Vertex vertex);
    void insert_edge(Vertex from, Vertex to, Weight weight = 1);
    void remove_vertex(Vertex vertex);
    void remove_edge(Vertex from, Vertex to);
    size_t get_vertex_number() const;
    size_t get_edge_number() const;
    const std::vector<Vertex> get_vertices() const;
    const std::vector<Edge> &get_adjacent_edges(Vertex vertex) const;
    const std::vector<Edge> &operator[](
        Vertex vertex) const;  // same as `get_adjacent_edges`

   private:
    size_t vertex_number = 0;
    size_t edge_number = 0;
    std::unique_ptr<std::unordered_map<Vertex, std::vector<Edge>>>
        adjacent_list;
};

template <typename Vertex, typename Weight>
Graph<Vertex, Weight>::Graph() {
    adjacent_list =
        std::make_unique<std::unordered_map<Vertex, std::vector<Edge>>>();
}

template <typename Vertex, typename Weight>
Graph<Vertex, Weight>::Graph(
    std::initializer_list<Vertex> vertices,
    std::initializer_list<std::pair<Vertex, Edge>> edges) {
    adjacent_list =
        std::make_unique<std::unordered_map<Vertex, std::vector<Edge>>>();

    // Insert vertices and make sure they are distinct.
    for (auto v : vertices) {
        (*adjacent_list)[v] = {};
    }
    if (auto n = adjacent_list->size(); n < vertices.size()) {
        throw std::runtime_error("Duplicated vertices.");
    } else {
        vertex_number = n;
    }

    // Insert edges.
    for (auto [v, e] : edges) {
        insert_edge(v, e.first, e.second);
    }
    edge_number = edges.size();
}

template <typename Vertex, typename Weight>
Graph<Vertex, Weight>::Graph(const Graph &graph) {
    vertex_number = graph.vertex_number;
    edge_number = graph.edge_number;
    adjacent_list =
        std::make_unique<std::unordered_map<Vertex, std::vector<Edge>>>(
            *(graph.adjacent_list));
}

template <typename Vertex, typename Weight>
Graph<Vertex, Weight>::Graph(Graph &&graph) {
    vertex_number = graph.vertex_number;
    edge_number = graph.edge_number;
    adjacent_list = std::move(graph.adjacent_list);
}

template <typename Vertex, typename Weight>
typename Graph<Vertex, Weight>::Graph &Graph<Vertex, Weight>::operator=(
    const Graph &graph) {
    vertex_number = graph.vertex_number;
    edge_number = graph.edge_number;
    adjacent_list =
        std::make_unique<std::unordered_map<Vertex, std::vector<Edge>>>(
            graph.adjacent_list);
    return *this;
}

template <typename Vertex, typename Weight>
typename Graph<Vertex, Weight>::Graph &Graph<Vertex, Weight>::operator=(
    Graph &&graph) {
    vertex_number = graph.vertex_number;
    edge_number = graph.edge_number;
    adjacent_list = std::move(graph.adjacent_list);
    return *this;
}

template <typename Vertex, typename Weight>
Graph<Vertex, Weight>::~Graph() {}

template <typename Vertex, typename Weight>
void Graph<Vertex, Weight>::insert_vertex(Vertex vertex) {
    auto [_, inserted] = adjacent_list->insert({vertex, {}});
    if (inserted) {
        ++vertex_number;
    }
}

template <typename Vertex, typename Weight>
void Graph<Vertex, Weight>::insert_edge(Vertex from, Vertex to, Weight weight) {
    // Make sure the start vertex exists.
    if (!adjacent_list->contains(from) || !adjacent_list->contains(to)) {
        throw std::runtime_error("Vertex does not exist.");
    }

    // Make sure the edge does not yet exist.
    auto &edges = (*adjacent_list)[from];
    auto it = std::find_if(edges.cbegin(), edges.cend(),
                           [to](auto edge) { return edge.first == to; });
    if (it != edges.cend()) {
        throw std::runtime_error("Duplicated edges.");
    }

    // Insert the edge.
    edges.push_back(Edge{to, weight});
    ++edge_number;
}

template <typename Vertex, typename Weight>
void Graph<Vertex, Weight>::remove_vertex(Vertex vertex) {
    if (!adjacent_list->contains(vertex)) {
        return;
    }

    // Remove the vertex and all edges starting from it.
    edge_number -= (*adjacent_list)[vertex].size();
    vertex_number -= adjacent_list->erase(vertex);

    // Remove all edges ending at this vertex.
    for (auto [_, vec] : *adjacent_list) {
        auto sz_before = vec.size();
        vec.erase(
            std::remove_if(vec.begin(), vec.end(),
                           [vertex](auto e) { return e.first == vertex; }),
            vec.end());
        auto sz_end = vec.size();
        edge_number -= sz_before - sz_end;
    }
}

template <typename Vertex, typename Weight>
void Graph<Vertex, Weight>::remove_edge(Vertex from, Vertex to) {
    if (!adjacent_list->contains(from)) {
        return;
    }
    auto vec = (*adjacent_list)[from];
    auto it = std::remove_if(vec.begin(), vec.end(),
                             [to](auto e) { return e.first == to; });
    if (it != vec.end()) {
        vec.erase(it, vec.end());
        --edge_number;
    }
}

template <typename Vertex, typename Weight>
size_t Graph<Vertex, Weight>::get_vertex_number() const {
    return vertex_number;
}

template <typename Vertex, typename Weight>
size_t Graph<Vertex, Weight>::get_edge_number() const {
    return edge_number;
}

template <typename Vertex, typename Weight>
const std::vector<Vertex> Graph<Vertex, Weight>::get_vertices() const {
    auto vertices = std::vector<Vertex>{};
    for (auto &[v, e] : *adjacent_list) {
        vertices.push_back(v);
    }
    return vertices;
}

template <typename Vertex, typename Weight>
const std::vector<typename Graph<Vertex, Weight>::Edge>
    &Graph<Vertex, Weight>::get_adjacent_edges(Vertex vertex) const {
    return (*adjacent_list)[vertex];
}

template <typename Vertex, typename Weight>
const std::vector<typename Graph<Vertex, Weight>::Edge>
    &Graph<Vertex, Weight>::operator[](Vertex vertex) const {
    return get_adjacent_edges(vertex);
}

#endif

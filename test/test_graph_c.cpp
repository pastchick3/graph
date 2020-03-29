#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "../src/graph_c.hpp"

TEST_CASE("graph_c") {
    auto graph = graph_ctor();

    graph_insert_vertex(graph, 1);
    graph_insert_vertex(graph, 2);
    graph_insert_vertex(graph, 3);
    graph_insert_vertex(graph, 4);
    graph_insert_edge(graph, 1, 2, 2);
    graph_insert_edge(graph, 2, 3, 1);
    graph_insert_edge(graph, 3, 4, 1);
    graph_insert_edge(graph, 4, 1, 1);

    REQUIRE(graph_get_vertex_number(graph) == 4);
    REQUIRE(graph_get_edge_number(graph) == 4);

    graph_remove_vertex(graph, 3);
    graph_remove_edge(graph, 4, 1);

    REQUIRE(graph_get_vertex_number(graph) == 3);
    REQUIRE(graph_get_edge_number(graph) == 1);

    auto vertices = graph_get_vertices(graph);
    REQUIRE(vertices->size == 3);
    graph_free_vertices(vertices);

    auto edges = graph_get_adjacent_edges(graph, 1);
    REQUIRE(edges->size == 1);
    REQUIRE(edges->array[0].to == 2);
    REQUIRE(edges->array[0].weight == 2);
    graph_free_edges(edges);

    graph_dtor(graph);
}

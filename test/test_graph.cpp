#include <stdexcept>
#include <utility>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "../src/graph.hpp"

TEST_CASE("graph") {
    auto graph = Graph<size_t, int>{{1, 2, 3},
                                    {
                                        {1, {2, 1}},
                                        {1, {3, 1}},
                                        {2, {3, 2}},
                                    }};
    REQUIRE(graph.get_vertex_number() == 3);
    REQUIRE(graph.get_edge_number() == 3);

    SECTION("construct") {
        {
            auto g = Graph{};
            REQUIRE(g.get_vertex_number() == 0);
            REQUIRE(g.get_edge_number() == 0);
        }
        {
            auto g = Graph{graph};
            REQUIRE(g.get_vertex_number() == 3);
            REQUIRE(g.get_edge_number() == 3);
        }
        {
            auto g = graph;
            REQUIRE(g.get_vertex_number() == 3);
            REQUIRE(g.get_edge_number() == 3);
        }
        {
            auto g = Graph{std::move(graph)};
            REQUIRE(g.get_vertex_number() == 3);
            REQUIRE(g.get_edge_number() == 3);
        }
        {
            auto g = std::move(graph);
            REQUIRE(g.get_vertex_number() == 3);
            REQUIRE(g.get_edge_number() == 3);
        }
    }

    SECTION("insert") {
        graph.insert_vertex(4);
        graph.insert_edge(1, 4);
        REQUIRE(graph.get_vertex_number() == 4);
        REQUIRE(graph.get_edge_number() == 4);
    }

    SECTION("remove") {
        graph.remove_vertex(2);
        graph.remove_edge(1, 3);
        REQUIRE(graph.get_vertex_number() == 2);
        REQUIRE(graph.get_edge_number() == 0);
    }

    SECTION("get") {
        auto vertics = graph.get_vertices();
        REQUIRE(vertics.size() == 3);
        auto edges = graph[2];
        REQUIRE(edges.size() == 1);
        REQUIRE(edges[0] == Graph<size_t, int>::Edge{3, 2});
    }

    SECTION("exception") {
        REQUIRE_THROWS_AS((Graph{{1, 1}, {}}), std::runtime_error);
        REQUIRE_THROWS_AS(graph.insert_edge(999, 999), std::runtime_error);
        REQUIRE_THROWS_AS(graph.insert_edge(1, 2), std::runtime_error);
    }
}

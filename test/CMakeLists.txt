add_executable(test_graph
    test_graph.cpp
)

target_link_libraries(test_graph
    graph
)

add_executable(test_graph_c
    test_graph_c.cpp
)

target_link_libraries(test_graph_c
    graph
)

add_library(graph STATIC
    graph.hpp
    graph.cpp
)

set_target_properties(graph PROPERTIES LINKER_LANGUAGE CXX)

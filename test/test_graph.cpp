//
// Created by maciej on 30.04.25.
//
// test_graph.cpp
#include <cassert>
#include <vector>
#include <string>
#include "graph.hpp"

int main() {
    std::vector<std::string> spec = {"ABCDE", "BCDEB", "CDEBA", "DEBAA"};
    int n = static_cast<int>(spec.size());

    graph::Graph g(n);
    g.build(spec);

    assert(g.neighbors(0).size() == 3);
    assert(g.neighbors(0)[0].first == 1);
    assert(g.neighbors(0)[0].second == 1);
    assert(g.neighbors(0)[1].first == 2);
    assert(g.neighbors(0)[1].second == 2);
    assert(g.neighbors(0)[2].first == 3);
    assert(g.neighbors(0)[2].second == 3);


    assert(g.size() == n);

    return 0;
}

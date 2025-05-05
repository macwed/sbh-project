//
// Created by maciej on 05.05.25.
//

#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP
#include <vector>

#include "graph.hpp"


std::vector<int> dijkstraPath (const graph::Graph& g, int src, int dest, const std::vector<bool>& visited);

#endif //DIJKSTRA_HPP

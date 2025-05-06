//
// Created by maciej on 02.05.25.
//

#ifndef GREEDY_HPP
#define GREEDY_HPP
#include <vector>

#include "graph.hpp"

std::vector<int> greedySolver(const graph::Graph& g, int start);

int computePathWeight(const graph::Graph& g, const std::vector<int>& path_vector);

std::vector<int> unvisitedNeighbors(const graph::Graph& g, int u, int weight, const std::vector<char>& visited);


#endif //GREEDY_HPP

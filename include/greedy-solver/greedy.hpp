//
// Created by maciej on 02.05.25.
//

#ifndef GREEDY_HPP
#define GREEDY_HPP
#include <vector>

#include "graph.hpp"

static constexpr int NUM_CANDIDATES = 5;
static constexpr double TOLERANCE = 1.2;
static constexpr double FINISH_RATIO = 0.95;


std::vector<int> greedySolver(const graph::Graph& g, int start);

int pathWeight(const graph::Graph& g, const std::vector<int>& path_vector);

std::vector<int> unvisitedNeighbors(const graph::Graph& g, int u, int weight, const std::vector<bool>& visited);


#endif //GREEDY_HPP

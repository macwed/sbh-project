//
// Created by maciej on 05.05.25.
//

#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP
#include <vector>

#include "graph.hpp"

constexpr int INF = std::numeric_limits<int>::max();

std::vector<int> dijkstraPath(const graph::Graph& g, int src, int dest);

std::vector<int> dijkstraPath (const graph::Graph& g, int src, int dest, const std::vector<char>& visited);

std::pair<std::vector<int>, std::vector<int>> dijkstraAll (const graph::Graph& g, int src);

std::pair<std::vector<int>, std::vector<int>> dijkstraAll (const graph::Graph& g, int src, const std::vector<char>& visited);

void finishRemaining (std::vector<int>& path, const graph::Graph& g,
                        std::vector<char>& visited, double finish_ratio = 1.0);

#endif //DIJKSTRA_HPP

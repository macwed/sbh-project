//
// Created by maciej on 05.05.25.
//

#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP
#include <vector>
#include <limits>

#include "graph.hpp"

constexpr int INF = std::numeric_limits<int>::max();

int findClosestUnvisited (const std::vector<int>& candidates, const std::vector<int>& dist);

std::vector<int> reconstructPath (int target, const std::vector<int>& prev);

std::vector<int> getUnvisited (const std::vector<char>& visited);

std::vector<int> dijkstraPath(const graph::Graph& g, int src, int dest);

std::vector<int> dijkstraPath (const graph::Graph& g, int src, int dest, const std::vector<char>& visited);

std::pair<std::vector<int>, std::vector<int>> dijkstraAll (const graph::Graph& g, int src);

std::pair<std::vector<int>, std::vector<int>> dijkstraAll (const graph::Graph& g, int src, const std::vector<char>& visited);

void finishRemaining (std::vector<int>& path, const graph::Graph& g, int& visited_count, std::vector<char>& visited);

#endif //DIJKSTRA_HPP

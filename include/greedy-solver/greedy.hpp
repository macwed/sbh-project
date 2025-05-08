//
// Created by maciej on 02.05.25.
//

#ifndef GREEDY_HPP
#define GREEDY_HPP
#include <vector>

#include "common/graph.hpp"

std::vector<int> greedySolver(const graph::Graph& g, int start);

int computePathWeight(const graph::Graph& g, const std::vector<int>& path_vector);

void selectCandidates(const graph::Graph& g, int current, int weight, std::vector<int>& candidates, const std::vector<char>& visited);

int stepBack (std::vector<int>& path, int& dead_end);

bool isDeadEnd (const graph::Graph& g, int u);

#endif //GREEDY_HPP

//
// Created by maciej on 02.05.25.
//

#ifndef GREEDY_HPP
#define GREEDY_HPP
#include <random>
#include <vector>

#include "common/graph.hpp"

using graph::Graph;
using Int_vec = std::vector<int>;
using Bool_vec = std::vector<char>;

Int_vec greedySolver(const Graph& g, int start);

int computePathWeight(const Graph& g, const Int_vec& path_vector);

int stepBack (Int_vec& path, int& dead_end);

bool isDeadEnd (const Graph& g, int u);

void goWeightOne(Int_vec& path, const Graph& g, Bool_vec& visited, int& visited_cnt);

std::vector<int> getNextCandidates(const Graph& g, int current_node, const Bool_vec& visited, std::mt19937_64& rng);

#endif //GREEDY_HPP

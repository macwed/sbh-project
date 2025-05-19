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
using Str_vec = std::vector<std::string>;

std::vector<int> greedySolver(Graph& g, int start, const Str_vec& spectrum, int positive_errors, int negative_errors);

int computePathWeight(const Graph& g, const Int_vec& path_vector);

int stepBack (Int_vec& path, int& dead_end);

int backToBranched (const Graph& g, Int_vec& path, Bool_vec& visited, Int_vec& returned_from, int& visited_count);

int backToBranchedW1(const Graph& g, Int_vec& path, Bool_vec& visited, Int_vec& returned_from, int& visited_count);

bool isDeadEnd(const Graph& g, int u);

void goWeightOne(Int_vec& path, const Graph& g, Bool_vec& visited, int& visited_cnt);

Int_vec getNextCandidates(const Graph& g, int current_node, const Bool_vec& visited, std::mt19937_64& rng, int visited_count);

Int_vec getNextCandidates (const Graph& g, int current_node,
    const Bool_vec& visited, std::mt19937_64& rng, const Int_vec& returned_from, int visited_count);

#endif //GREEDY_HPP

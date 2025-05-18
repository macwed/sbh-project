//
// Created by maciej on 02.05.25.
//

#include "greedy-solver/greedy.hpp"

#include <algorithm>
#include <iostream>
#include <random>
#include <ranges>

#include "common/graph.hpp"
#include "common/dijkstra.hpp"

static constexpr int CAND_NUM = 4;
//static constexpr int NUM_CANDIDATES_RESELECT = 5;
static constexpr double TOLERANCE = 1.2;
static constexpr double FINISH_RATIO = 0.95;

using graph::Graph;
using Int_vec = std::vector<int>;
using Bool_vec = std::vector<char>;

std::vector<int> greedySolver(const Graph& g, int start) {

    int n = g.size();
    Int_vec path;
    Bool_vec visited(n, false);
    int path_size = static_cast<int>(TOLERANCE * n);
    path.reserve(path_size);
    int current = start;
    visited[current] = true;
    int visited_count = 1;
    path.emplace_back(current);
    std::mt19937_64 rng(std::random_device{}());
    //int dead_end = -1;

    while (true) {

        goWeightOne(path, g, visited, visited_count);
        current = path.back();
        Int_vec candidates = getNextCandidates(g, current, visited, rng);
        int best_candidate = -1;
        int best_distance = INF;
        Int_vec subpath;
        subpath.reserve(n / 10);
        for (const auto& candidate : candidates) {
            auto sp = dijkstraPath(g, current, candidate, visited);
            int dist = computePathWeight(g, sp);
            if (!sp.empty() && dist < best_distance) {
                best_candidate = candidate;
                best_distance = dist;
                std::cout << "Debug: Chosen unvisited vertex: " << best_candidate << " distance: " << best_distance << std::endl;
            }
        }
        if (best_candidate == -1 || best_distance == INF) {
            std::cout << "Dijkstra through visited. Current: " << current << std::endl;

            for (int u : candidates) {
                auto sp = dijkstraPath(g, current, u);
                int dist = computePathWeight(g, sp);
                if (!sp.empty() && dist < best_distance) {
                    best_candidate = u;
                    best_distance = dist;
                    std::cout << "Debug: Chosen (un)visited vertex: " << best_candidate << " distance: " << best_distance << std::endl;
                }
            }
        }
        if ((best_candidate == -1 || best_distance == INF) && (1.0 * visited_count / n) >= FINISH_RATIO) {
            //still no path. Try to finish the sequence with DijkstraAll.
            //Consider reselecting neighbors few times before finishing...???
            std::cout << "Debug: no path found to candidates!" << std::endl;
            //std::cout << "Trying to finish the sequence with finishRemaining..." << std::endl;
            //finishRemaining(path, g, visited_count, visited);
            break;
        }
        subpath = dijkstraPath(g, current, best_candidate, visited);
        //chosen = best_u;

        for (size_t i = 1; i < subpath.size(); ++i) {
            int u = subpath[i];
            visited[u] = true;
            ++visited_count;
            path.emplace_back(u);
        }
        current = path.back();
    }

    return path;
}

int computePathWeight(const Graph& g, const Int_vec& path_vector) {
    if (path_vector.size() < 2) return INF;
    int path_weight = 0;

    for (size_t i = 0; i + 1 < path_vector.size(); ++i) {
        int weight = g.weight(path_vector[i], path_vector[i + 1]);
        if (!weight) return INF;
        path_weight += weight;
    }
    return path_weight;
}

int stepBack (Int_vec& path, int& dead_end) {
    if (path.size() < 2) {
        // cant go back anymore
        dead_end = -1;
        return -1;
    }
    dead_end = path.back();
    int previous_node = *(path.end() - 2);
    path.pop_back();
    std::cout << "Stepback! From " << dead_end << " to " << previous_node << std::endl;

    return previous_node;
}

bool isDeadEnd (const Graph& g, int u) {
    return g.neighbors(u).empty();
}

void goWeightOne(Int_vec& path, const Graph& g, Bool_vec& visited, int& visited_cnt) {

    if (path.empty()) {
        std::cerr << "Empty path" << std::endl;
        return;
    }
    int current_node = path.back();
    bool moved = true;
    while (moved) {
        moved = false;
        for (auto& [v, w] : g.neighbors(current_node)) {
            if (!visited[v] && w == 1) {
                current_node = v;
                path.push_back(current_node);
                visited[v] = 1;
                ++visited_cnt;
                moved = true;
                break;
            }
        }
    }
}

Int_vec getNextCandidates(const Graph& g, int current_node, const Bool_vec& visited, std::mt19937_64& rng) {

    Int_vec nextCandidates;
    nextCandidates.reserve(CAND_NUM);
    for (const auto &v1: g.neighbors(current_node) | std::views::keys) {
        if (visited[v1]) continue;
        bool allNeighborsUnvisited = true;
        for (const auto &v2: g.neighbors(v1) | std::views::keys) {
            if (visited[v2]) allNeighborsUnvisited = false;
            break;
        }
        if (allNeighborsUnvisited) nextCandidates.push_back(v1);
    }
    if (nextCandidates.size() >= CAND_NUM) return nextCandidates;

    Int_vec unvisited;
    unvisited.reserve(visited.size());
    for (int i = 0; i < static_cast<int>(visited.size()); ++i)
        if (!visited[i]) unvisited.push_back(i);
    std::ranges::shuffle(unvisited, rng);
    for (auto u : unvisited) {
        nextCandidates.push_back(u);
        if (nextCandidates.size() >= CAND_NUM) return nextCandidates;
    }
    return nextCandidates;
}
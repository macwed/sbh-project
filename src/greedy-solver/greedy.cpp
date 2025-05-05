//
// Created by maciej on 02.05.25.
//

#include "greedy-solver/greedy.hpp"

#include <algorithm>
#include <random>
#include <ranges>

#include "graph.hpp"
#include "dijkstra.hpp"

using graph::Graph;

std::vector<int> greedySolver(const Graph& g, int start) {

    int n = g.size();
    std::vector<int> path;
    std::vector<bool> visited(n, false);
    int path_size = static_cast<int>(TOLERANCE * n);
    path.reserve(path_size);
    int current = start;
    visited[current] = true;
    path.emplace_back(current);
    std::mt19937_64 rng(std::random_device{}());

    while (true) {
        auto next1 = unvisitedNeighbors(g, current, 1, visited);
        if (!next1.empty()) {
            current = next1.front();
            visited[current] = true;
            path.emplace_back(current);
            continue;
        }
        std::vector<int> candidates;
        candidates.reserve(NUM_CANDIDATES * 2);
        for (auto wgt : {2, 3}) {
            candidates = std::move(unvisitedNeighbors(g, current, wgt, visited));
            if (candidates.size() >= NUM_CANDIDATES) break;
        }
        if (candidates.size() < NUM_CANDIDATES) {
            std::vector<int> pool;
            pool.reserve(n);
            for (int u = 0; u < n; ++u) if (!visited[u]) {
                for (auto [v, w] : g.neighbors(u)) {
                    if (!visited[v] && w == 1) {
                        pool.emplace_back(v);
                        break;
                    }
                }
            }
            std::ranges::shuffle(pool, rng);
            for (int u : pool) {
                if (candidates.size() >= NUM_CANDIDATES) break;
                candidates.emplace_back(u);
            }
        }
        if (candidates.empty()) break;

        int chosen = -1;
        for (int u : candidates) {
            for (auto v: g.neighbors(current) | std::views::keys) {
                if (v == u) {
                    chosen = u;
                    break;
                }
            }
            if (chosen != -1) break;
        }
        std::vector<int> subpath;
        subpath.reserve(n / 10);
        if (chosen == -1) {
            int best_u = -1;
            int best_dist = std::numeric_limits<int>::max();
            for (int u : candidates) {
                auto sp = dijkstraPath(g, current, u, visited);
                int dist = pathWeight(g, sp);
                if (!sp.empty() && dist < best_dist) {
                    best_u = u;
                    best_dist = dist;
                }
            }
            if (best_u == -1) break; //no path
            subpath = dijkstraPath(g, current, best_u, visited);
            chosen = best_u;
        }
        for (int i = 1; i < subpath.size(); ++i) {
            int u = subpath[i];
            visited[u] = true;
            path.emplace_back(u);
        }
        current = chosen;

        int visited_count = static_cast<int>(std::ranges::count(visited, true));
        if (1.0 * visited_count / n >= FINISH_RATIO) {
            //finishRemaining;
        }
    }

    return path;
}

std::vector<int> unvisitedNeighbors(const Graph& g, int u, int weight, const std::vector<bool>& visited) {
    std::vector<int> unv_neighbors;
    for (auto [v, w] : g.neighbors(u)) {
        if (!visited[v] && w == weight) unv_neighbors.push_back(v);
    }
    return unv_neighbors;
}

int pathWeight(const Graph& g, const std::vector<int>& path_vector) {
    if (path_vector.size() < 2) {
        throw std::invalid_argument("Path vector is empty");
    }
    int path_weight = 0;

    for (int i = 0; i + 1 < path_vector.size(); ++i) {
        int weight = g.weight(path_vector[i], path_vector[i + 1]);
        if (!weight) {
            throw std::invalid_argument("Edge not found " + std::to_string(i) + " -> " + std::to_string(i + 1));
        }
        path_weight += weight;
    }
    return path_weight;
}

//WORK IN PROGRESS
//std::vector<int> finishRemaining () {}
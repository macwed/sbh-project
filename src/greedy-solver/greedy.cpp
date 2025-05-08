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

static constexpr int NUM_CANDIDATES = 8;
//static constexpr int NUM_CANDIDATES_RESELECT = 5;
static constexpr double TOLERANCE = 1.2;
static constexpr double FINISH_RATIO = 0.95;

using graph::Graph;

std::vector<int> greedySolver(const Graph& g, int start) {

    int n = g.size();
    std::vector<int> path;
    std::vector<char> visited(n, false);
    int path_size = static_cast<int>(TOLERANCE * n);
    path.reserve(path_size);
    int current = start;
    visited[current] = true;
    int visited_count = 1;
    path.emplace_back(current);
    std::mt19937_64 rng(std::random_device{}());
    int dead_end = -1;

    while (true) {
        if (current == -1) {
            std::cout << "StepBack failed: cannot go back further.\n";
            break; // or return path;
        }
        //Choose edges with weight 1
        bool moved = true;
        while (moved) {
            moved = false;
            for (auto [v, w] : g.neighbors(current)) {
                if (!visited[v] && w == 1) {
                    current = v;
                    visited[current] = true;
                    ++visited_count;
                    path.emplace_back(current);
                    moved = true;
                    break;
                }
            }
        }

        if (dead_end != -1) {
            visited[dead_end] = false;
            --visited_count;
            dead_end = -1;
        }
        if (isDeadEnd(g, current) && (1.0 * visited_count / n) < FINISH_RATIO) {
            current = stepBack(path, dead_end);
            continue;
        }

        std::cout << "\n--- Debug Check ---\n";
        std::cout << "Current vertex: " << current << "\n";

        std::cout << "Visited[10] = " << static_cast<int>(visited[10]) << "\n";
        std::cout << "Neighbors of 5: ";
        for (auto [v, w] : g.neighbors(5)) {
            std::cout << "(" << v << ", " << w << ")";
            if (v == 10) std::cout << " <-- target!";
            std::cout << " ";
        }
        std::cout << "\n";

        std::cout << "Current neighbors (of " << current << "): ";
        for (auto [v, w] : g.neighbors(current)) {
            std::cout << "(" << v << ", " << w << ") ";
        }
        std::cout << "\n";

        std::cout << "--- End Debug ---\n\n";


        //fallback: no weight 1 edge from current node - choose candidates from neighbors
        //that has w1 edge to farther unvisited nodes
        std::vector<int> candidates;
        candidates.reserve(NUM_CANDIDATES * 2);
        selectCandidates(g, current, 1, candidates, visited);

        //randomly add candidates from other unvisited nodes that has weight 1 edge to other unvisited node
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
        /*if (candidates.empty()) {
            std::cout << "Debug: Empty candidates! Break algorithm loop 1" << std::endl; //debug
            break;
        }*/

        //prefer candidates from neighbors of the current node
        int chosen = -1;
        for (int u : candidates) {
            for (auto v: g.neighbors(current) | std::views::keys) {
                if (v == u) {
                    chosen = u;
                    break;
                }
            }
            if (chosen != -1) {
                std::cout << "Debug: Chosen vertex " << chosen << " at " << current << std::endl;
                path.emplace_back(chosen);
                break;
            }
        }
        //no candidates from neighbors selected.
        //Use Dijkstra's algorithm to select nearest candidate (subpath with the least weight), unvisited nodes only
        std::vector<int> subpath;
        subpath.reserve(n / 10);
        if (chosen == -1) {
            std::cout << "Dijkstra through unvisited. Current: " << current << std::endl;
            int best_u = -1;
            int best_dist = INF;
            for (int u : candidates) {
                auto sp = dijkstraPath(g, current, u, visited);
                int dist = computePathWeight(g, sp);
                if (!sp.empty() && dist < best_dist) {
                    best_u = u;
                    best_dist = dist;
                    std::cout << "Debug: Chosen unvisited vertex: " << best_u << " distance: " << best_dist << std::endl;
                }
            }
            //no possible path via unvisited nodes only. Allow visited nodes.
            if (best_u == -1 || best_dist == INF) {
                std::cout << "Dijkstra through visited. Current: " << current << std::endl;

                for (int u : candidates) {
                    auto sp = dijkstraPath(g, current, u);
                    int dist = computePathWeight(g, sp);
                    if (!sp.empty() && dist < best_dist) {
                        best_u = u;
                        best_dist = dist;
                        std::cout << "Debug: Chosen (un)visited vertex: " << best_u << " distance: " << best_dist << std::endl;
                    }
                }
            }
            if ((best_u == -1 || best_dist == INF) && (1.0 * visited_count / n) >= FINISH_RATIO) {
                //still no path. Try to finish the sequence with DijkstraAll.
                //Consider reselecting neighbors few times before finishing...???
                std::cout << "Debug: no path found to candidates!" << std::endl;
                std::cout << "Trying to finish the sequence with finishRemaining..." << std::endl;
                finishRemaining(path, g, visited_count, visited);
                break;
            }
            subpath = dijkstraPath(g, current, best_u, visited);
            chosen = best_u;
        }
        for (size_t i = 1; i < subpath.size(); ++i) {
            int u = subpath[i];
            visited[u] = true;
            ++visited_count;
            path.emplace_back(u);
        }
        current = chosen;

        visited_count = static_cast<int>(std::ranges::count(visited, true));

        if (1.0 * visited_count / n >= FINISH_RATIO) {
            finishRemaining(path, g, visited_count, visited);
            break;
        }
    }

    return path;
}

int computePathWeight(const Graph& g, const std::vector<int>& path_vector) {
    if (path_vector.size() < 2) return INF;
    int path_weight = 0;

    for (size_t i = 0; i + 1 < path_vector.size(); ++i) {
        int weight = g.weight(path_vector[i], path_vector[i + 1]);
        if (!weight) return INF;
        path_weight += weight;
    }
    return path_weight;
}

void selectCandidates(const Graph& g, int current, int weight, std::vector<int>& candidates, const std::vector<char>& visited) {
    candidates.reserve(NUM_CANDIDATES * 2);
    for (auto v1 : g.neighbors(current) | std::views::keys) {
        for (auto [v2, w2] : g.neighbors(v1)) {
            if (!visited[v2] && w2 == weight) {
                candidates.emplace_back(v1);
                break;
            }
        }
    }
}

int stepBack (std::vector<int>& path, int& dead_end) {
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
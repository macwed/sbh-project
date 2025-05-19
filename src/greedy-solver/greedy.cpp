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

static constexpr int CAND_RATIO = 5;
static constexpr int MAX_LOOPS = 50;
//static constexpr int NUM_CANDIDATES_RESELECT = 5;
static constexpr double TOLERANCE = 1.2;
static constexpr double FINISH_RATIO = 0.95;
static constexpr int STEP_BACKS = 8;
static constexpr int RESELECT = 5;

using graph::Graph;
using Int_vec = std::vector<int>;
using Bool_vec = std::vector<char>;
using Str_vec = std::vector<std::string>;

namespace {
    bool allVisited(int visited_count, int total_nodes) {
        return visited_count == total_nodes;
    }

    bool overTolerance(size_t path_size, int total_nodes) {
        return path_size >= static_cast<size_t>(static_cast<double>(total_nodes) * TOLERANCE);
    }

    bool shouldFinishEarly(int best_candidate, int best_distance, int visited_count, int total_nodes) {
        return (best_candidate == -1 || best_distance == INF) &&
               (static_cast<double>(visited_count) / total_nodes) >= FINISH_RATIO;
    }

    bool gotCandidate(int best_candidate, int best_distance, int visited_count, int total_nodes) {
        return (best_candidate != -1 && best_distance != INF);
    }

    bool reachedFinishRatio(int visited_count, int total_nodes) {
        return static_cast<double>(visited_count) / total_nodes >= FINISH_RATIO;
    }

    bool shouldSkipDeadEnd(int visited_count, int total_nodes, int pos_errors) {
        return visited_count < total_nodes - pos_errors;
    }
}

void addVirtualEdge (Graph& g, const Str_vec& spectrum, int current_node,
                        int virtual_weight, std::mt19937_64& rng, const Bool_vec& visited) {
    int n = static_cast<int>(g.size());
    int k = static_cast<int>(spectrum[0].size());
    std::vector<std::string_view> views;
    views.reserve(n);
    for (const auto& s : spectrum) {
        views.emplace_back(s);
    }
    Int_vec candidates;
    candidates.reserve(n);
    auto base_curr = views[current_node].data();
    for (int i = 0; i < n; ++i) {
        auto base_i = views[i].data();
        if (std::string_view(base_curr + virtual_weight, k - virtual_weight)
            == std::string_view(base_i, k - virtual_weight) && !visited[i]) {
            candidates.push_back(i);
        }
    }
    //prefer candidates with unvisited neighbors of weight 1
    bool found = false;
    candidates.shrink_to_fit();
    std::ranges::shuffle(candidates.begin(), candidates.end(), rng);
    for (auto& cand : candidates) {
        for (auto& [v, w] : g.neighbors(cand)) {
            if (!visited[v] && w == 1) {
                g.addEdge(current_node, cand, virtual_weight);
                found = true;
                break;
            }
        }
        if (found) break;
    }
    //pick any candidate with unvisited neighbor
    if (!found) {
        for (auto& cand : candidates) {
            for (const auto &v: g.neighbors(cand) | std::views::keys) {
                if (!visited[v]) {
                    g.addEdge(current_node, cand, virtual_weight);
                    found = true;
                    break;
                }
            }
            if (found) break;
        }
    }
}

std::vector<int> greedySolver(Graph& g, int start, const Str_vec& spectrum, int positive_errors, int negative_errors) {

    int n = g.size();
    Int_vec path;
    Bool_vec visited(n, false);
    int max_path_size = static_cast<int>(TOLERANCE * n);
    path.reserve(max_path_size);
    int current = start;
    visited[current] = true;
    int visited_count = 1;
    path.emplace_back(current);
    std::mt19937_64 rng(std::random_device{}());
    int loops = 0;

    while (true) {
        if (allVisited(visited_count, n)
            || overTolerance(path.size(), n)
            || reachedFinishRatio(visited_count, n))
            break;

        goWeightOne(path, g, visited, visited_count);
        current = path.back();

        Int_vec been_there;
        if (isDeadEnd(g, current) && loops < MAX_LOOPS) {
            std::cout << "First stepback!" << std::endl;
            current = backToBranchedW1(g, path, visited, been_there, visited_count);
        }

        if (current == -1 && loops < MAX_LOOPS) {
            std::cout << "Second stepback!" << std::endl;
            current = backToBranched(g, path, visited, been_there, visited_count);
        }

        if (current == -1 || (isDeadEnd(g, current) && loops >= MAX_LOOPS)) {
            std::cout << "Virtual Edge!" << std::endl;
            current = path.back();
            addVirtualEdge(g, spectrum, current, 4, rng, visited);
        }

        int best_candidate = -1;
        int best_distance = INF;
        Int_vec subpath;
        subpath.reserve(n / 10);
        int cnt_reselect = 0;
        while (cnt_reselect < RESELECT) {
            Int_vec candidates;
            if (shouldSkipDeadEnd(visited_count, n, positive_errors))
                candidates = getNextCandidates(g, current, visited, rng, been_there, visited_count);
            else
                candidates = getNextCandidates(g, current, visited, rng, visited_count);

            for (const auto& candidate : candidates) {
                auto sp = dijkstraPath(g, current, candidate, visited);
                int dist = computePathWeight(g, sp);
                if (!sp.empty() && dist < best_distance) {
                    best_candidate = candidate;
                    best_distance = dist;
                    std::cout << "Debug: Chosen unvisited vertex: " << best_candidate << " distance: " << best_distance << std::endl;
                }
            }
            if (gotCandidate(best_candidate, best_distance, visited_count, n)) break;

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
            if (gotCandidate(best_candidate, best_distance, visited_count, n)) break;
            if (shouldFinishEarly(best_candidate, best_distance, visited_count, n)) {
                //still no path. Try to finish the sequence with DijkstraAll.
                //Consider reselecting neighbors few times before finishing...???
                std::cout << "Debug: no path found to candidates!" << std::endl;
                //std::cout << "Trying to finish the sequence with finishRemaining..." << std::endl;
                //finishRemaining(path, g, visited_count, visited);
                break;
            }
            ++cnt_reselect;
        }
        subpath = dijkstraPath(g, current, best_candidate, visited);
        //chosen = best_u;

        for (size_t i = 1; i < subpath.size(); ++i) {
            int u = subpath[i];
            visited[u] = true;
            ++visited_count;
            path.emplace_back(u);
        }
        loops++;
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

//step back to the nearest node with any unvisited neighbors
int backToBranched (const Graph& g, Int_vec& path, Bool_vec& visited, Int_vec& returned_from, int& visited_count) {

    int current_node = path.back();
    bool foundBranch = false;
    int step_cnt = 0;
    while (!foundBranch) {
        if (path.size() < 2) {
            std::cout << "Debug: Back to branched returned: path.size < 2" << std::endl;
            return -1;
        }
        //too many stepbacks, reverse changes in path, return -1
        if (step_cnt >= STEP_BACKS) {
            std::cout << "Debug: Back to branched returned!\nstepBacks: " << step_cnt << std::endl;
            std::ranges::reverse(returned_from);
            for (auto& v : returned_from) {
                path.emplace_back(v);
                visited[v] = true;
                ++visited_count;
            }
            return -1;
        }
        visited[current_node] = 0;
        --visited_count;
        returned_from.push_back(current_node);
        path.pop_back();
        current_node = path.back();
        for (const auto &v1: g.neighbors(current_node) | std::views::keys) {
            bool beenThere = std::ranges::find(returned_from, v1) != returned_from.end();
            if (!visited[v1] && !beenThere) {
                foundBranch = true;
                break;
            }
        }
        step_cnt++;
    }
    return current_node;
}

//step back to the nearest node with unvisited neighbors with an arc of weight 1
int backToBranchedW1 (const Graph& g, Int_vec& path, Bool_vec& visited, Int_vec& returned_from, int& visited_count) {
    if (path.size() < 2) {
        return -1;
    }
    int current_node = path.back();
    bool foundBranch = false;
    for (auto& v : path)
        if (g.nodeDegree(v) > 1) {
            foundBranch = true;
            break;
        }
    if (!foundBranch) return -1;
    while (g.nodeDegree(current_node) <= 1) {
        visited[current_node] = 0;
        --visited_count;
        returned_from.push_back(current_node);
        path.pop_back();
        current_node = path.back();
    }
    return current_node;
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
//get candidates for Dijkstra path
Int_vec getNextCandidates(const Graph& g, int current_node, const Bool_vec& visited, std::mt19937_64& rng, int visited_count) {

    Int_vec nextCandidates;
    int cand_num = (static_cast<int>(g.size()) - visited_count) / CAND_RATIO;
    nextCandidates.reserve(cand_num);
    for (const auto &v1: g.neighbors(current_node) | std::views::keys) {
        if (visited[v1]) continue;
        bool allNeighborsUnvisited = true;
        for (const auto &v2: g.neighbors(v1) | std::views::keys) {
            if (visited[v2]) {
                allNeighborsUnvisited = false;
                break;
            }
        }
        if (allNeighborsUnvisited) nextCandidates.push_back(v1);
    }
    if (nextCandidates.size() >= cand_num) return nextCandidates;

    Int_vec unvisited;
    unvisited.reserve(visited.size());
    for (int i = 0; i < static_cast<int>(visited.size()); ++i)
        if (!visited[i]) unvisited.push_back(i);
    std::ranges::shuffle(unvisited, rng);
    for (auto u : unvisited) {
        nextCandidates.push_back(u);
        if (nextCandidates.size() >= cand_num) return nextCandidates;
    }
    return nextCandidates;
}
//get candidates for Dijkstra path but ignore last dead-end path
Int_vec getNextCandidates (const Graph& g, int current_node,
    const Bool_vec& visited, std::mt19937_64& rng, const Int_vec& returned_from, int visited_count) {

    Int_vec nextCandidates;
    int cand_num = (static_cast<int>(g.size()) - visited_count) / CAND_RATIO;
    nextCandidates.reserve(cand_num);
    for (const auto &v1: g.neighbors(current_node) | std::views::keys) {
        if (visited[v1] || std::ranges::find(returned_from, v1) != returned_from.end()) continue;
        bool allNeighborsUnvisited = true;
        for (const auto &v2: g.neighbors(v1) | std::views::keys) {
            if (visited[v2]) {
                allNeighborsUnvisited = false;
                break;
            }
        }
        if (allNeighborsUnvisited) nextCandidates.push_back(v1);
    }
    if (nextCandidates.size() >= cand_num) return nextCandidates;

    Int_vec unvisited;
    unvisited.reserve(visited.size());
    for (int i = 0; i < static_cast<int>(visited.size()); ++i)
        if (!visited[i]) unvisited.push_back(i);
    std::ranges::shuffle(unvisited, rng);
    for (auto u : unvisited) {
        nextCandidates.push_back(u);
        if (nextCandidates.size() >= cand_num) return nextCandidates;
    }
    return nextCandidates;
}
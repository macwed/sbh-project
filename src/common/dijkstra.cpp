//
// Created by maciej on 05.05.25.
//
#include "common/dijkstra.hpp"

#include <algorithm>
#include <queue>
#include "common/graph.hpp"

using Pair = std::pair<int, int>;

//Dijkstra algorithm including visited nodes
std::vector<int> dijkstraPath(const graph::Graph& g, int src, int dest) {

    if (src == dest) return {src};

    int n = g.size();
    std::vector<int> dist (n, INF);
    std::vector<int> prev (n, -1);

    std::priority_queue<Pair, std::vector<Pair>, std::greater<>> pq;
    dist[src] = 0;
    pq.emplace(dist[src], src);

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;

        for (auto [v, w] : g.neighbors(u)) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
                pq.emplace(dist[v], v);
            }
        }
    }
    if (dist[dest] == INF) {
        return {};
    }
    std::vector<int> path;
    for (int at = dest; at != -1; at = prev[at]) {
        path.push_back(at);
    }
    std::ranges::reverse(path);
    return path;
}

//Dijkstra algorithm through unvisited nodes
std::vector<int> dijkstraPath (const graph::Graph& g, int src, int dest, const std::vector<char>& visited) {

    if (src == dest) return {src};

    int n = g.size();
    std::vector<int> dist (n, INF);
    std::vector<int> prev (n, -1);

    std::priority_queue<Pair, std::vector<Pair>, std::greater<>> pq;
    dist[src] = 0;
    pq.emplace(dist[src], src);

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;

        for (auto [v, w] : g.neighbors(u)) {
            if (visited[v]) continue;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
                pq.emplace(dist[v], v);
            }
        }
    }
    if (dist[dest] == INF) {
        return {};
    }
    std::vector<int> path;
    for (int at = dest; at != -1; at = prev[at]) {
        path.push_back(at);
    }
    std::ranges::reverse(path);
    return path;
}

std::pair<std::vector<int>, std::vector<int>> dijkstraAll (const graph::Graph& g, int src) {

    int n = g.size();
    std::vector<int> dist (n, INF);
    std::vector<int> prev (n, -1);

    std::priority_queue<Pair, std::vector<Pair>, std::greater<>> pq;
    dist[src] = 0;
    pq.emplace(dist[src], src);
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;

        for (auto [v, w] : g.neighbors(u)) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
                pq.emplace(dist[v], v);
            }
        }
    }
    return {dist, prev};
}

std::pair<std::vector<int>, std::vector<int>> dijkstraAll (const graph::Graph& g, int src, const std::vector<char>& visited) {

    int n = g.size();
    std::vector<int> dist (n, INF);
    std::vector<int> prev (n, -1);

    std::priority_queue<Pair, std::vector<Pair>, std::greater<>> pq;
    dist[src] = 0;
    pq.emplace(dist[src], src);
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;

        for (auto [v, w] : g.neighbors(u)) {
            if (visited[v]) continue;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
                pq.emplace(dist[v], v);
            }
        }
    }
    return {dist, prev};
}

int findClosestUnvisited (const std::vector<int>& candidates, const std::vector<int>& dist) {
    int best_u = -1, best_dist = INF;

    for (int u : candidates) {
        if (dist[u] < best_dist) {
            best_u = u;
            best_dist = dist[u];
        }
    }
    return best_u;
}

std::vector<int> reconstructPath (int target, const std::vector<int>& prev) {
    std::vector<int> path;
    for (int at = target; at != -1; at = prev[at]) {
        path.push_back(at);
    }
    std::ranges::reverse(path);
    return path;
}

std::vector<int> getUnvisited (const std::vector<char>& visited) {
    std::vector<int> result;
    for (int i = 0; i < static_cast<int>(visited.size()); ++i) {
        if (!visited[i]) result.push_back(i);
    }
    return result;
}

void finishRemaining (std::vector<int>& path, const graph::Graph& g, int& visited_count, std::vector<char>& visited) {

    int current = path.back();

    while (true) {
        std::vector<int> remaining = getUnvisited(visited);
        if (remaining.empty()) break;

        auto [dist, prev] = dijkstraAll(g, current, visited);
        int best_u = findClosestUnvisited(remaining, dist);

        if (best_u == -1 || dist[best_u] == INF) {
            std::tie(dist, prev) = dijkstraAll(g, current);
            best_u = findClosestUnvisited(remaining, dist);
        }

        if (best_u == -1 || dist[best_u] == INF) break; // IN THE MIDDLE OF NOWHERE ! IT'S OVER!

        std::vector<int> subpath = reconstructPath(best_u, prev);
        for (size_t i = 1; i < subpath.size(); ++i) {
            int v = subpath[i];
            visited[v] = true;
            ++visited_count;
            path.push_back(v);
        }

        current = best_u;
    }
}
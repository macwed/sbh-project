//
// Created by maciej on 05.05.25.
//
#include "dijkstra.hpp"

#include <algorithm>
#include <limits>
#include <queue>

#include "graph.hpp"

using Pair = std::pair<int, int>;

constexpr int INF = std::numeric_limits<int>::max();

//Dijkstra algorithm including visited nodes
std::vector<int> dijkstraPath(const graph::Graph& g, int src, int dest) {

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


void finishRemaining (std::vector<int>& path, const graph::Graph& g,
                        std::vector<char>& visited, double finish_ratio) {

    int n = g.size();
    int current = path.back();

    while (true) {
        std::vector<int> remaining;
        for (int i = 0; i < n; ++i)
            if (!visited[i])
                remaining.push_back(i);
        if (remaining.empty()) break;

        int visited_count = static_cast<int>(std::ranges::count(visited, true));
        if (static_cast<double> (visited_count) / n >= finish_ratio) break;

        auto [dist, prev] = dijkstraAll(g, current, visited);

        int best_u = -1;
        int best_dist = INF;
        for (int u : remaining) {
            if (dist[u] < best_dist) {
                best_dist = dist[u];
                best_u = u;
            }
        }

        if (best_u == -1 || best_dist == INF) {
            std::tie(dist, prev) = dijkstraAll(g, current);
            for (int u : remaining) {
                if (dist[u] < best_dist) {
                    best_dist = dist[u];
                    best_u = u;
                }
            }
        }
        if (best_u == -1 || best_dist == INF) break;

        std::vector<int> subpath;
        for (int v = best_u; v != -1; v = prev[v]) {
            subpath.push_back(v);
        }
        std::ranges::reverse(subpath);

        for (size_t i = 1; i < subpath.size(); ++i) {
            int v = subpath[i];
            path.push_back(v);
            visited[v] = true;
        }

        current = path.back();
    }
}
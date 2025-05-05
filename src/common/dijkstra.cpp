//
// Created by maciej on 05.05.25.
//
#include "dijkstra.hpp"

#include <algorithm>
#include <limits>
#include <queue>

#include "graph.hpp"

using Pair = std::pair<int, int>;

std::vector<int> dijkstraPath (const graph::Graph& g, int src, int dest, const std::vector<bool>& visited) {

    int n = g.size();
    std::vector<int> dist (n, std::numeric_limits<int>::max());
    std::vector<int> prev (n, -1);

    std::priority_queue<Pair, std::vector<Pair>, std::greater<>> pq;
    dist[src] = 0;
    pq.emplace(dist[src], src);

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;
        if (u == dest) break;

        for (auto [v, w] : g.neighbors(u)) {
            if (visited[v]) continue;
            int nd = d + w; //new distance
            if (nd < dist[v]) {
                dist[v] = nd;
                prev[v] = u;
                pq.emplace (nd, v);
            }
        }
    }
    if (dist[dest] == std::numeric_limits<int>::max()) {
        return {};
    }
    std::vector<int> path;
    for (int at = dest; at != -1; at = prev[at]) {
        path.push_back(at);
    }
    std::ranges::reverse(path);
    return path;
}

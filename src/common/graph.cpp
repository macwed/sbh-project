//
// Created by maciej on 22.04.25.
//

#include "graph.hpp"

#include <algorithm>
#include <vector>
#include <string>
#include <string_view>

namespace graph {

    Graph::Graph(unsigned int node_count)
        : adj_(node_count), out_degree1_(node_count, 0) {
        for (auto& v : adj_) v.reserve(10);
    }

    int Graph::size() const {
        return static_cast<int>(adj_.size());
    }

    int Graph::nodeDegree(int u) const {

        return out_degree1_[u];
    }

    void Graph::addEdge(unsigned int u, unsigned int v, unsigned int w) {
        adj_[u].emplace_back(v, w);
        if (w == 1) ++out_degree1_[u];
    }

    void Graph::resize(int node_count) {

        adj_.assign(node_count, {});
        for (auto& v : adj_) v.reserve(10);
        out_degree1_.assign(node_count, 0);
    }

    void Graph::build(const std::vector<std::string>& spectrum) {
        int n = static_cast<int>(spectrum.size());
        if (n == 0) return;
        int k = static_cast<int>(spectrum[0].size());
        std::vector<std::string_view> views;
        views.reserve(n);
        for (const auto& s : spectrum) {
            views.emplace_back(s);
        }

        resize(n);
        for (int i = 0; i < n; ++i) {
            auto base_i = views[i].data();
            for (int j = 0; j < n; ++j) {
                if (i == j) continue;
                auto base_j = views[j].data();
                for (int w = 1; w <= MAX_WEIGHT; ++w) {
                    if (std::string_view(base_i + w, k - w) == std::string_view(base_j, k - w)) {
                        addEdge(i, j, w);
                        break;
                    }
                }
            }
        }
    }
    const std::vector<std::pair<int, int>>& Graph::neighbors(int u) const {

        return adj_[u];
    }

    int Graph::weight(int u, int dest) const {
        for (auto [v, w] : neighbors(u)) {
            if (dest == v) return w;
        }
        return 0;
    }

    bool Graph::hasEdge(int u, int dest) const {
        return std::ranges::any_of(adj_[u], [dest](const auto& edge) {
            return edge.first == dest;
        });
    }
}
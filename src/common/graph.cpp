//
// Created by maciej on 22.04.25.
//

#include "graph.hpp"
#include <string_view>

namespace graph {

    Graph::Graph(int node_count)
            : adj_(node_count), out_degree1_(node_count, 0) {}

    void Graph::addEdge(unsigned int u, unsigned int v, unsigned int w) {
        adj_[u].emplace_back(v, w);
        if (w == 1) ++out_degree1_[u];
    }

    void Graph::build(const std::vector<std::string>& spectrum) {
        size_t n = spectrum.size();
        if (n == 0) return;
        size_t k = spectrum[0].size();
        resize(n);

        std::vector<std::string_view> views;
        views.reserve(n);
        for (const auto& s : spectrum) {
            views.emplace_back(s);
        }

        for (size_t i = 0; i < n; i++) {
            auto base_i = views[i].data();
            for (size_t j = 0; j < n; j++) {
                if (std::string_view(base_i+1, k - 1) == std::string_view(views[j].data(), k - 1)) {
                    addEdge(i, j, 1);
                }
                else if (std::string_view(base_i+2, k - 2) == std::string_view(views[j].data(), k - 2)) {
                    addEdge(i, j, 2);
                }
                else if (std::string_view(base_i+3, k - 3) == std::string_view(views[j].data(), k - 3)) {
                    addEdge(i, j, 3);
                }
            }
        }
    }

    void Graph::resize(size_t node_count) {
        adj_.assign(node_count, {});
        out_degree1_.assign(node_count, 0);
    }

    void Graph::clear() {
        resize(static_cast<int>(adj_.size()));
    }

    const std::vector<std::pair<int, int>>& Graph::neighbors(int u) const {
        return adj_[u];
    };

    int Graph::size() const {
        return static_cast<int>(adj_.size());
    }

}
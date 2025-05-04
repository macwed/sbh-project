//
// Created by maciej on 22.04.25.
//

#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <vector>
#include <utility>

namespace graph {
    using adj_list = std::vector<std::vector<std::pair<int,int>>>;

    class Graph {
      private:
        adj_list adj_;
        std::vector<int> out_degree1_;
      public:
        explicit Graph(unsigned int node_count = 0);

        void addEdge(unsigned int u, unsigned int v, unsigned int w);

        [[nodiscard]] int nodeDegree(int u) const;

        void build(const std::vector<std::string>& spectrum);

        void resize(int node_count);

        [[nodiscard]] const std::vector<std::pair<int, int>>& neighbors(int u) const;

        [[nodiscard]] int size() const;
        };
    };

#endif //GRAPH_HPP

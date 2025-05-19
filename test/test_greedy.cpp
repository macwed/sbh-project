#include <iostream>
#include <vector>
#include <string>

#include "common/graph.hpp"
#include "greedy-solver/greedy.hpp"
#include "generator/io.hpp"

void printPath(const std::vector<int>& path) {
    std::cout << "Path: ";
    for (int v : path) std::cout << v << " ";
    std::cout << "\n";
}

void printCoverage(const std::vector<char>& visited) {
    int total = static_cast<int>(visited.size());
    int visited_count = 0;
    for (char v : visited) if (v) ++visited_count;

    std::cout << "Visited: " << visited_count << "/" << total
              << " (" << 100.0 * visited_count / total << "%)\n";
}

int main() {
    std::string filename = "../bin/500_10_0_0.txt";

    int n, k, neg, pos;
    std::vector<std::string> spectrum;

    try {
        io::readSpectrumFromFile(filename, n, k, neg, pos, spectrum);
    } catch (const std::exception& e) {
        std::cerr << "Błąd wczytywania pliku: " << e.what() << std::endl;
        return 1;
    }

    graph::Graph g;
    g.build(spectrum);

    std::vector<int> path = greedySolver(g, 0, spectrum, pos, neg);  // jaki będzie primer?

    std::vector<char> visited(g.size(), false);
    for (int v : path) visited[v] = true;

    printPath(path);
    std::cout << "Total weight: " << computePathWeight(g, path) << "\n";
    printCoverage(visited);

    return 0;
}

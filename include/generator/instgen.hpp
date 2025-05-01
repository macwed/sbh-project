//
// Created by maciej on 14.04.25.
//

#ifndef SEQ_GEN_HPP
#define SEQ_GEN_HPP
#include <random>
#include <string>
#include <vector>

namespace instgen {
    using sv = std::vector<std::string>;

    void generateZeroErrorSequence(int n, int k, std::mt19937& gen, std::string& sequence);

    void generateRandomSequence (int n, std::mt19937& gen, std::string& sequence);

    void generateSpectrumFromSequence(int k, const std::string& sequence, sv& spectrum);

    void addNegativeErrors(sv& spectrum, const std::string& sequence, int negative_errors_int, std::mt19937& gen);

    void addPositiveErrors (sv& spectrum, int positive_errors_int, std::mt19937& gen);
}

#endif //SEQ_GEN_HPP

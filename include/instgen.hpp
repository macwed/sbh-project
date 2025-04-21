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

    void generate_zero_error_sequence(int n, int k, std::mt19937& gen, std::string& sequence);

    void generate_random_sequence (int n, std::mt19937& gen, std::string& sequence);

    void generate_spectrum_from_sequence(int k, const std::string& sequence, sv& spectrum);

    void add_negative_errors(sv& spectrum, const std::string& sequence, int negative_errors_int, std::mt19937& gen);

    void add_positive_errors (sv& spectrum, int positive_errors_int, std::mt19937& gen);
}

#endif //SEQ_GEN_HPP

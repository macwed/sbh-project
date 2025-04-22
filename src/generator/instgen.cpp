//
// Created by maciej on 14.04.25.
//

#include "../include/generator/instgen.hpp"

#include <algorithm>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

namespace instgen {
    using sv = std::vector<std::string>;
    using umap = std::unordered_map<std::string, int>;

    void generate_zero_error_sequence(int n, int k, std::mt19937& gen, std::string& sequence) {
        /*-------------spektrum bez błędów-------------*/

        std::string acgt = {"ACGT"};
        std::uniform_int_distribution<size_t> dis(0, acgt.size() - 1);
        for (int i = 0; i < n; i++) {
            sequence += acgt[dis(gen)];
        }
        bool is_negative_error;
        do {
            is_negative_error = false;
            for (int i = 0; i < sequence.length(); i++) {
                std::string s1 = sequence.substr(i, k);
                for (int j = 0; j < sequence.length(); j++) {
                    if (i == j) continue;
                    std::string s2 = sequence.substr(j, k);
                    if (s1 == s2) {
                        char new_base = acgt[0];
                        while (new_base == s2.back()) {
                            new_base = acgt[dis(gen)];
                        }
                        sequence[j + k - 1] = new_base;
                        is_negative_error = true;
                    }
                }
            }
        } while (is_negative_error);
    }

    void generate_random_sequence (int n, std::mt19937& gen, std::string& sequence) {
        std::string acgt = {"ACGT"};
        std::uniform_int_distribution<size_t> dis(0, acgt.size() - 1);
        for (int i = 0; i < n; i++) {
            sequence += acgt[dis(gen)];
        }
    }

    void generate_spectrum_from_sequence(int k, const std::string& sequence, sv& spectrum) {
        for (int i = 0; i <= sequence.length() - k; i++) {
            spectrum.emplace_back(sequence.substr(i, k));
        }
        std::ranges::sort(spectrum);
    }

    void add_negative_errors(sv& spectrum, const std::string& sequence, int negative_errors_int, std::mt19937& gen) {


        umap check_negative_errors;
        for (const auto& s : spectrum) {
            check_negative_errors[s]++;
        }
        int error_count = static_cast<int>(spectrum.size() - check_negative_errors.size());
        std::string primer = sequence.substr(0,spectrum[0].length());

        for (int i = error_count; i < negative_errors_int; i++) {
            std::uniform_int_distribution<size_t> dis(0, spectrum.size() - 1);
            int index = static_cast<int>(dis(gen));
            std::string s = spectrum[index];
            if (s != primer && check_negative_errors[s] == 1) {
                spectrum.erase(spectrum.begin() + index);
            }
        }
    }

    void add_positive_errors (sv& spectrum, int positive_errors_int, std::mt19937& gen) {

        std::string acgt = {"ACGT"};
        for (int i = 0; i < positive_errors_int; i++) {
            std::uniform_int_distribution<size_t> dis(0, spectrum.size() - 1);
            int index = static_cast<int>(dis(gen));
            std::string s = spectrum[index];
            size_t mid = s.length() / 2;
            if (s.length() % 2) {                             //jeżeli długość s jest nieparzysta
                std::uniform_int_distribution<size_t> dis2 (0, acgt.size() - 1);
                char new_base = acgt[dis2(gen)];
                while (s[mid] == new_base) {
                    new_base = acgt[dis2(gen)];
                }
                s[mid] = new_base;
                spectrum.push_back(s);
            }
            else {
                std::uniform_int_distribution<size_t> dis2 (0, acgt.size() - 1);
                char new_base1 = acgt[dis2(gen)];
                char new_base2 = acgt[dis2(gen)];
                while (s[mid-1] == new_base1 || s[mid] == new_base2) {
                    new_base1 = acgt[dis2(gen)];
                    new_base2 = acgt[dis2(gen)];
                }
                s[mid-1] = new_base1;
                s[mid] = new_base2;
                spectrum.push_back(s);
            }
        }
    }
}


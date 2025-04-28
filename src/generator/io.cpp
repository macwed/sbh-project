//
// Created by maciej on 15.04.25.
//

#include "generator/io.hpp"

#include <fstream>
#include <vector>
#include <string>
#include <filesystem>

using vs = std::vector<std::string>;
namespace fs = std::filesystem;
namespace io {
    void read_from_file(const std::string& filename, std::string& main_sequence) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Nie mozna odczytac pliku z sekwencja");
        }
        file >> main_sequence;
        file.close();
    }

    void write_to_file_sequence(const std::string& main_sequence, const std::string& filepath) {
        std::string filename = filepath + std::to_string(main_sequence.size()) + "_seq.txt";
        std::ofstream file2(filename);
        if (!file2.is_open()) {
            throw std::runtime_error("Nie mozna utworzyc pliku do zapisu sekwencji");
        }
        file2 << main_sequence;
        file2.close();
    }

    void write_to_file_spectrum(const vs& spectrum, int n, int k, int negative_errors,
                                int positive_errors, const std::string& filepath) {

        std::string filename = filepath + std::to_string(n) + "_" + std::to_string(k)
            + "_" + std::to_string(negative_errors) + "_" + std::to_string(positive_errors) + ".txt";
        std::ofstream file1(filename);
        if (!file1.is_open()) {
            throw std::runtime_error("Nie mozna utworzyc pliku do zapisu spektrum");
        }
        file1 << n << "\n" << k << "\n" << negative_errors << "\n" << positive_errors << std::endl;
        for (const auto& s : spectrum) {
            file1 << s << std::endl;
        }
        file1.close();
    }

    void writeInstance(const std::string& sequence,
                       const std::vector<std::string>& spectrum,
                       int n, int k, int negErr, int posErr) {
        write_to_file_sequence(sequence, "");
        write_to_file_spectrum(spectrum, n, k, negErr, posErr, "");
    }
}
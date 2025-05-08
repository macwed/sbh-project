//
// Created by maciej on 15.04.25.
//

#ifndef READ_WRITE_HPP
#define READ_WRITE_HPP
#include <string>
#include <vector>

namespace io {
    using vs = std::vector<std::string>;

    void readFromFile(const std::string& filename, std::string& main_sequence);

    void readSpectrumFromFile(const std::string& filename, int& n, int& k, int& negErr, int& posErr, std::vector<std::string>& spectrum);

    void writeToFileSpectrum(const vs& spectrum, int n, int k, int negative_errors,
                                int positive_errors, const std::string& filepath);

    void writeToFileSequence(const std::string& main_sequence, const std::string& filepath);

    void writeInstance(const std::string& sequence,
                       const std::vector<std::string>& spectrum,
                       int n, int k, int negErr, int posErr);
}

#endif //READ_WRITE_HPP

//
// Created by maciej on 20.04.25.
//

#include "runner.hpp"

#include <cli.hpp>
#include <io.hpp>
#include <random>
#include <instgen.hpp>

namespace runner {
    void runSingle(const cli::Config& cfg) {
        std::mt19937 gen(std::random_device{}());
        std::string sequence;
        sequence.reserve(cfg.n);
        instgen::generate_random_sequence(cfg.n, gen, sequence);

        std::vector<std::string> spectrum;
        spectrum.reserve(cfg.n - cfg.k + 1);
        instgen::generate_spectrum_from_sequence(cfg.k, sequence, spectrum);

        if (cfg.negErr > 0) {
            instgen::add_negative_errors(spectrum, sequence, cfg.negErr, gen);
        }
        if (cfg.posErr > 0) {
            instgen::add_positive_errors(spectrum, cfg.posErr, gen);
        }

        io::writeInstance(sequence, spectrum, cfg.n, cfg.k, cfg.negErr, cfg.posErr);
    }

    void runFull() {
        const int instanceCount = 50;
        std::random_device rd;

        for (int nVal = 250; nVal <= 700; nVal += 50) {
            for (int i = 0; i < instanceCount; ++i) {
                std::mt19937 gen(rd());
                std::string sequence;
                sequence.reserve(nVal);
                instgen::generate_zero_error_sequence(nVal, 7, gen, sequence);

                for (int kVal = 7; kVal <= 10; ++kVal) {
                    std::vector<std::string> spectrum;
                    spectrum.reserve(nVal - kVal + 1);
                    instgen::generate_spectrum_from_sequence(kVal, sequence, spectrum);
                    io::writeInstance(sequence, spectrum, nVal, kVal, 0, 0);
                }

                for (int kVal = 7; kVal <= 10; ++kVal) {
                    std::vector<std::string> baseSpectrum;
                    baseSpectrum.reserve(nVal - kVal + 1);
                    instgen::generate_spectrum_from_sequence(kVal, sequence, baseSpectrum);
                    int total = static_cast<int>(baseSpectrum.size());
                    for (int percent = 2; percent <= 18; percent += 2) {
                        int errCount = static_cast<int>((percent / 100.0) * total);
                        auto spec = baseSpectrum;
                        instgen::add_negative_errors(spec, sequence, errCount, gen);
                        io::writeInstance(sequence, spec, nVal, kVal, errCount, 0);
                    }
                }

                for (int kVal = 7; kVal <= 10; ++kVal) {
                    std::vector<std::string> baseSpectrum;
                    baseSpectrum.reserve(nVal - kVal + 1);
                    instgen::generate_spectrum_from_sequence(kVal, sequence, baseSpectrum);
                    int total = static_cast<int>(baseSpectrum.size());
                    for (int percent = 2; percent <= 18; percent += 2) {
                        int errCount = static_cast<int>((percent / 100.0) * total);
                        auto spec = baseSpectrum;
                        instgen::add_positive_errors(spec, errCount, gen);
                        io::writeInstance(sequence, spec, nVal, kVal, 0, errCount);
                    }
                }
            }
        }
    }
}
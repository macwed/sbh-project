//
// Created by maciej on 20.04.25.
//
#include <random>
#include <filesystem>
#include <omp.h>
#include "generator/runner.hpp"
#include "generator/io.hpp"
#include "generator/instgen.hpp"
#include "generator/cli.hpp"

namespace runner {
    namespace fs = std::filesystem;

    void runSingle(const cli::Config& cfg) {
        std::mt19937 gen(std::random_device{}());
        std::string sequence;
        sequence.reserve(cfg.n);
        instgen::generateRandomSequence(cfg.n, gen, sequence);

        std::vector<std::string> spectrum;
        spectrum.reserve(cfg.n - cfg.k + 1);
        instgen::generateSpectrumFromSequence(cfg.k, sequence, spectrum);

        if (cfg.negErr > 0) {
            instgen::addNegativeErrors(spectrum, sequence, cfg.negErr, gen);
        }
        if (cfg.posErr > 0) {
            instgen::addPositiveErrors(spectrum, cfg.posErr, gen);
        }

        io::writeInstance(sequence, spectrum, cfg.n, cfg.k, cfg.negErr, cfg.posErr);
    }

    void runFull(const cli::Config& cfg) {

        const fs::path baseDir = cfg.outputDir.empty()
          ? "test_instances"
          : cfg.outputDir;
        fs::create_directories(baseDir);

        #pragma omp parallel for schedule(dynamic, 1) default(none) shared(cfg, baseDir)
        for (int inst = 1; inst <= cfg.instanceCount; ++inst) {
            std::random_device rd;
            std::mt19937 gen(rd() + inst + omp_get_thread_num());
            std::ostringstream oss;
            oss << "instance_" << std::setw(2) << std::setfill('0') << inst;
            fs::path instDir = baseDir / oss.str();
            fs::create_directory(instDir);

            for (int n = cfg.minN; n <= cfg.maxN; n += cfg.stepN) {
                fs::path nDir = instDir / ("n" + std::to_string(n));
                fs::create_directory(nDir);

                std::string seq;
                instgen::generateZeroErrorSequence(n, cfg.kMin, gen, seq);

                for (int k = cfg.kMin; k <= cfg.kMax; ++k) {
                    fs::path kDir = nDir / ("k" + std::to_string(k) + "/");
                    fs::create_directory(kDir);

                    std::vector<std::string> spectrum;
                    instgen::generateSpectrumFromSequence(k, seq, spectrum);
                    io::writeToFileSequence(seq, (kDir).string());
                    io::writeToFileSpectrum(spectrum, n, k, 0, 0,
                                               (kDir).string());

                    double total = static_cast<double>(spectrum.size());
                    for (int pct = cfg.errorStep; pct <= cfg.errorMax; pct += cfg.errorStep) {

                        int errCount = static_cast<int>((pct/100.0) * total);

                        auto specNeg = spectrum;
                        instgen::addNegativeErrors(specNeg, seq, errCount, gen);
                        io::writeToFileSpectrum(specNeg, n, k, errCount, 0,
                                                   (kDir).string());

                        auto specPos = spectrum;
                        instgen::addPositiveErrors(specPos, errCount, gen);
                        io::writeToFileSpectrum(specPos, n, k, 0, errCount,
                                                   (kDir).string());
                    }
                }
            }
        }
    }
}

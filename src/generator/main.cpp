#include <iostream>
#include <ostream>

#include "generator/cli.hpp"
#include "generator/runner.hpp"

int main(int argc, char* argv[]) {
    try {
        auto cfg = cli::parseArgs(argc, argv);
        if (cfg.mode == cli::Config::Single) {
            runner::runSingle(cfg);
        } else {
            runner::runFull(cfg);
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}

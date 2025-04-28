//
// Created by maciej on 20.04.25.
//

#ifndef RUNNER_HPP
#define RUNNER_HPP
#include "cli.hpp"

namespace runner {
    void runSingle(const cli::Config& cfg);
    void runFull(const cli::Config& cfg);
}

#endif //RUNNER_HPP

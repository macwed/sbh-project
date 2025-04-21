//
// Created by maciej on 18.04.25.
//

#include "cli.hpp"

#include <../include/cxxopts.hpp>
#include <iostream>

namespace cli {
    Config parseArgs(int argc, char* argv[]) {
        Config cfg;
        cxxopts::Options options("instance_gen", "SBH instance generator");
        options.add_options()
            ("m,mode", "Mode: single or full", cxxopts::value<std::string>()->default_value("single"))
            ("s,size", "Sequence length",           cxxopts::value<int>()->default_value("500"))
            ("k,lmer", "l-mer length",             cxxopts::value<int>()->default_value("10"))
            ("n,neg",  "Negative errors count",      cxxopts::value<int>()->default_value("0"))
            ("p,pos",  "Positive errors count",      cxxopts::value<int>()->default_value("0"))
            ("h,help", "Print help message");
        auto result = options.parse(argc, argv);
        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            std::exit(0);
        }
        std::string mode = result["mode"].as<std::string>();
        cfg.mode   = (mode == "full" ? Config::Full : Config::Single);
        cfg.n      = result["size"].as<int>();
        cfg.k      = result["lmer"].as<int>();
        cfg.negErr = result["neg"].as<int>();
        cfg.posErr = result["pos"].as<int>();
        return cfg;
    }
}

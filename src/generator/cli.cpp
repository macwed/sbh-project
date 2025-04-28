// Created by maciej on 18.04.25.

#include "generator/cli.hpp"
#include <cxxopts.hpp>
#include <iostream>

namespace cli {

Config parseArgs(int argc, char* argv[]) {
    Config cfg;

    cxxopts::Options options("instance_gen", "SBH instance generator");
    options.add_options()
        ("m,mode",     "Mode: single or full",                          cxxopts::value<std::string>()->default_value("single"))
        ("s,size",     "Sequence length (single mode)",                 cxxopts::value<int>()->default_value("500"))
        ("l,lmer",     "l-mer length (single)",                         cxxopts::value<int>()->default_value("10"))
        ("n,neg",      "Negative error count (single)",                 cxxopts::value<int>()->default_value("0"))
        ("p,pos",      "Positive error count (single)",                 cxxopts::value<int>()->default_value("0"))
        ("o,outdir",   "Output directory for full mode",                cxxopts::value<std::string>()->default_value("test_instances"))
        ("c,count",    "Number of instances to generate",               cxxopts::value<int>()->default_value("50"))
        ("min",        "Min sequence length for full mode",            cxxopts::value<int>()->default_value("250"))
        ("max",        "Max sequence length for full mode",            cxxopts::value<int>()->default_value("700"))
        ("step",       "Step of sequence length for full mode",         cxxopts::value<int>()->default_value("50"))
        ("kmin",       "Min l-mer length for full mode",               cxxopts::value<int>()->default_value("7"))
        ("kmax",       "Max l-mer length for full mode",               cxxopts::value<int>()->default_value("10"))
        ("errStep",    "Error % step for full mode",                    cxxopts::value<int>()->default_value("2"))
        ("errMax",     "Error % max for full mode",                     cxxopts::value<int>()->default_value("18"))
        ("h,help",     "Show help");

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        std::exit(0);
    }

    cfg.mode   = (result["mode"].as<std::string>() == "full" ? Config::Full : Config::Single);

    // single
    cfg.n      = result["size"].as<int>();
    cfg.k      = result["lmer"].as<int>();
    cfg.negErr = result["neg"].as<int>();
    cfg.posErr = result["pos"].as<int>();

    // full
    cfg.outputDir     = result["outdir"].as<std::string>();
    cfg.instanceCount = result["count"].as<int>();
    cfg.minN          = result["min"].as<int>();
    cfg.maxN          = result["max"].as<int>();
    cfg.stepN         = result["step"].as<int>();
    cfg.kMin          = result["kmin"].as<int>();
    cfg.kMax          = result["kmax"].as<int>();
    cfg.errorStep     = result["errStep"].as<int>();
    cfg.errorMax      = result["errMax"].as<int>();

    return cfg;
}

}

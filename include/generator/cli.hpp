//
// Created by maciej on 18.04.25.
//

#ifndef CLI_HPP
#define CLI_HPP

#include <string>

namespace cli {

    struct Config {
        enum Mode { Single, Full } mode = Single;

        // single
        int n      = 500;
        int k      = 10;
        int negErr = 0;
        int posErr = 0;

        // full
        std::string outputDir     = "test_instances";
        int         instanceCount = 50;
        int         minN          = 250;
        int         maxN          = 700;
        int         stepN         = 50;
        int         kMin          = 7;
        int         kMax          = 10;
        int         errorStep     = 2;   // percent
        int         errorMax      = 18;  // percent
    };

    Config parseArgs(int argc, char* argv[]);

}

#endif //CLI_HPP

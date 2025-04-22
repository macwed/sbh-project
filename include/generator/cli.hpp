//
// Created by maciej on 18.04.25.
//

#ifndef CLI_HPP
#define CLI_HPP

namespace cli {
    struct Config {
        enum Mode { Single, Full } mode = Single;
        int n = 500;
        int k = 10;
        int negErr = 0;
        int posErr = 0;
    };

    Config parseArgs(int argc, char* argv[]);
}

#endif //CLI_HPP

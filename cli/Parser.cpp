//
// Created by Ahmed Mustafa on 1/23/2026.
//

#include "Parser.h"

namespace Split
{

    std::pair<std::string, std::vector<std::string>> Parser::Parse(const int argc, char** argv)
    {
        if (argc < 2)
            return {"", {}};

        std::string command = argv[1];
        std::vector<std::string> args;

        for (int i = 2; i < argc; ++i)
            args.emplace_back(argv[i]);

        return {command, args};
    }


}

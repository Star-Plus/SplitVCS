//
// Created by Ahmed Mustafa on 1/23/2026.
//

#ifndef SPLITVCS_PARSER_H
#define SPLITVCS_PARSER_H
#include <vector>

#include "core/Alias.h"


namespace Split
{
    class Parser
    {
    public:
        static std::pair<str, std::vector<str>> Parse(int argc, char** argv);
    };
}


#endif //SPLITVCS_PARSER_H
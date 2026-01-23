//
// Created by Ahmed Mustafa on 1/23/2026.
//

#ifndef SPLITVCS_COMMAND_H
#define SPLITVCS_COMMAND_H
#include "core/Alias.h"
#include <vector>

namespace Split
{

    class Command
    {
    public:
        virtual ~Command() = default;
        Command() = default;

        virtual str name() const = 0;
        virtual str description() const = 0;
        virtual str usage() const = 0;

        virtual int run(const std::vector<str>& args) = 0;
    };

}

#endif //SPLITVCS_COMMAND_H
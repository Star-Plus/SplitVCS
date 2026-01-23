//
// Created by Ahmed Mustafa on 1/23/2026.
//

#ifndef SPLITVCS_STREAMCOMMAND_H
#define SPLITVCS_STREAMCOMMAND_H
#include <fstream>
#include <ios>

#include "core/Alias.h"
#include <vector>

namespace Split
{

    class OutStreamCommand
    {
    protected:
        int save(const std::vector<str>& args, const str& buffer)
        {
            auto lastIndex = args.size() - 1;
            if (args.size() > 3 && args[lastIndex-1] == ">")
            {
                std::fstream out(args[lastIndex], std::ios::out | std::ios::binary);
                out << buffer;
                out.close();
                return 0;
            }

            return 1;
        }
    };

}

#endif //SPLITVCS_STREAMCOMMAND_H
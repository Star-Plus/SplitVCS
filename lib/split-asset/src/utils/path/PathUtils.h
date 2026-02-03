//
// Created by Ahmed Mustafa on 2/2/2026.
//

#ifndef SPLITVCS_PATHUTILS_H
#define SPLITVCS_PATHUTILS_H

#include <string>

namespace Split
{
    class PathUtils
    {
    public:
        static void removeSuffix(std::string& path, const std::string& suffix);
    };
} // Split

#endif //SPLITVCS_PATHUTILS_H
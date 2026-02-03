//
// Created by Ahmed Mustafa on 2/2/2026.
//

#include "PathUtils.h"

#include <iostream>
#include <ostream>

namespace Split
{
    void PathUtils::removeSuffix(std::string& path, const std::string& suffix)
    {
        if (path.ends_with(suffix))
        {
            path.erase(path.size() - suffix.size());
        }
    }
} // Split
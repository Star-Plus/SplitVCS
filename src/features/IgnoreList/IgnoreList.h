//
// Created by Ahmed Mustafa on 1/22/2026.
//

#ifndef SPLITVCS_IGNORELIST_H
#define SPLITVCS_IGNORELIST_H

#include <string>
#include <set>
#include "main/Logger.h"

namespace Split
{
    class IgnoreList
    {
    public:
        explicit IgnoreList(const std::string& rootPath);

        void add(const std::string& path);
        void remove(const std::string& path);

        bool isIgnored(const std::string& path) const;


    private:
        std::set<std::string> ignoreList;
        const std::string ignoreFilePath;
        Logger logger;

        void save() const;
    };
}
#endif //SPLITVCS_IGNORELIST_H
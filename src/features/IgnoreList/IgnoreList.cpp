//
// Created by Ahmed Mustafa on 1/22/2026.
//

#include "IgnoreList.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <ranges>

#include "utils/String/StringUtils.h"

namespace Split
{
    IgnoreList::IgnoreList(const std::string& rootPath) : path(rootPath + "/.split/.split.ignore")
    {
        std::filesystem::create_directories(rootPath+"/.split/");

        std::fstream file(path, std::ios::in);

        if (!file.is_open())
        {
            file.open(path, std::ios::out);
            file.close();
            return;
        }

        std::string line;
        while (std::getline(file, line))
        {
            ignoreList.insert(line);
        }

        file.close();
    }

    void IgnoreList::add(const std::string& path)
    {
        ignoreList.insert(path);
        save();
    }

    void IgnoreList::remove(const std::string& path)
    {
        ignoreList.erase(path);
        save();
    }

    bool IgnoreList::isIgnored(const std::string& path) const
    {
        auto parts = StringUtils::split(path, "/\\");

        for (auto part : parts)
        {
            std::cout << part.data() << std::endl;
            if (ignoreList.contains(part.data())) return true;
        }

        return false;
    }

    void IgnoreList::save() const
    {
        std::ofstream file(path);

        for (auto ignoreItem : ignoreList)
        {
            file << ignoreItem.data() << "\n";
        }

        file.close();
    }
}

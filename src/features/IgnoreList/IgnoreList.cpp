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
    IgnoreList::IgnoreList(const std::string& rootPath) : ignoreFilePath(rootPath + "/.split/.split.ignore"), logger(true)
    {
        std::filesystem::create_directories(rootPath+"/.split/");

        std::fstream file(ignoreFilePath, std::ios::in);

        if (!file.is_open())
        {
            file.open(ignoreFilePath, std::ios::out);
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

        logger.debug("Size of parts: " + std::to_string(parts.size()));

        for (const auto& part : parts)
        {
            logger.debug(part);
            if (ignoreList.contains(part)) return true;
        }

        return false;
    }

    void IgnoreList::save() const
    {
        std::ofstream file(ignoreFilePath);

        for (auto ignoreItem : ignoreList)
        {
            file << ignoreItem.data() << "\n";
        }

        file.close();
    }
}

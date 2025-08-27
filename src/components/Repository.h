//
// Created by Ahmed Mustafa on 8/13/2025.
//

#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <string>

#include "stores/CommitHistory.h"
#include "stores/Index.h"

namespace Split {
    class Repository {

        std::string rootPath;
        Index index;
        CommitHistory history;

    public:
        Repository(const std::string& rootPath);

        void init() const;
        void add(const std::string& filepath);
        void commit(const std::string& message, const std::string& author);
        void checkout(const std::string& commitHash);
        std::vector<str> getCommitHistory() const;

        const Index& getIndex() const { return index; }
        const str getRootPath() const { return rootPath; }

    };
}

#endif //REPOSITORY_H

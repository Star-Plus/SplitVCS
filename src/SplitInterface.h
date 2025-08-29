//
// Created by Ahmed Mustafa on 8/18/2025.
//

#ifndef SPLITINTERFACE_H
#define SPLITINTERFACE_H

#include "core/Alias.h"
#include <vector>

namespace Split {

    class SplitInterface {
    public:
        static void Init(const str& rootPath);
        static void Add(const str& rootPath, const str& filePath);
        static void Commit(const str& rootPath, const str& commitMessage, const str& author);
        static void Checkout(const str& rootPath, const str& commitId);

        static str Negotiate(const str& repoPath, const str& requestedCommitId);

        static str GetStatus(const str& rootPath);
        static std::vector<str> GetCommitHistory(const str& rootPath);

    };

}




#endif //SPLITINTERFACE_H

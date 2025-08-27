//
// Created by Ahmed Mustafa on 8/26/2025.
//

#ifndef COMMITDIFFER_H
#define COMMITDIFFER_H

#include <components/Repository.h>

#include "DIFF_TYPE.h"

namespace Split {

    struct CommitDiff {
        DiffType type;
        str filePath;
        str hash;
    };

    class CommitDiffer {
        Repository repo;

    public:
        CommitDiffer(const str& repoPath) : repo(repoPath) {}

        std::vector<CommitDiff> calculateDiffs(const str &commitA_Hash, const str &commitB_Hash) const;
    };

}




#endif //COMMITDIFFER_H

//
// Created by Ahmed Mustafa on 8/26/2025.
//

#include "CommitDiffer.h"

#include <unordered_map>

#include "components/Tree.h"

namespace Split {
    std::vector<CommitDiff> CommitDiffer::calculateDiffs(const str &commitA_Hash, const str &commitB_Hash) const {

        ObjectStore commitStore(repo.getRootPath(), "/commits");
        ObjectStore treeStore(repo.getRootPath(), "/trees");

        auto commitAObj = commitStore.loadObject(commitA_Hash);
        auto commitBObj = commitStore.loadObject(commitB_Hash);

        auto commitA = Commit::deserialize(commitAObj);
        auto commitB = Commit::deserialize(commitBObj);

        auto treeAObj = treeStore.loadObject(commitA.treeHash);
        auto treeBObj = treeStore.loadObject(commitB.treeHash);

        auto treeA = Tree::deserialize(treeAObj);
        auto treeB = Tree::deserialize(treeBObj);

        std::vector<CommitDiff> diffs;

        std::unordered_map<str, str> treeAMap;
        for (const auto& entry : treeA.getEntries()) {
            treeAMap[entry.first] = entry.second;
        }

        std::unordered_map<str, str> treeBMap;
        for (const auto& entry : treeB.getEntries()) {
            treeBMap[entry.first] = entry.second;
        }

        // Check for additions and modifications
        for (const auto& [filePath, hashB] : treeBMap) {
            if (treeAMap.find(filePath) == treeAMap.end()) {
                diffs.push_back({DiffType::ADDED, filePath, hashB});
            } else if (treeAMap[filePath] != hashB) {
                diffs.push_back({DiffType::MODIFIED, filePath, hashB});
            }
        }

        // Check for deletions
        for (const auto& [filePath, hashA] : treeAMap) {
            if (treeBMap.find(filePath) == treeBMap.end()) {
                diffs.push_back({DiffType::REMOVED, filePath, hashA});
            }
        }

        return diffs;
    }

}

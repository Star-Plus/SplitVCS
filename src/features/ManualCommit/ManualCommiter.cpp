#include "ManualCommiter.h"
#include "stores/ObjectStore.h"
#include "stores/CommitHistory.h"

namespace Split {

    ManualCommiter::ManualCommiter(const str& repoPath) : repoPath(repoPath) {}

    ManualCommiter::~ManualCommiter() {}

    void ManualCommiter::uploadTree(const str& treeHash, const str& treeContent) {
        auto treeStore = ObjectStore(repoPath, "trees");
        treeStore.storeBytesObject(treeContent);
    }

    void ManualCommiter::uploadCommit(const str& commitHash, const str& commitContent) {
        auto commitStore = ObjectStore(repoPath, "commits");
        commitStore.storeBytesObject(commitContent);
        auto commitHistory = CommitHistory(repoPath);
        commitHistory.addCommit(commitHash);
    }

}
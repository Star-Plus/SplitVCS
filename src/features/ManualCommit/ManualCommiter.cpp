#include "ManualCommiter.h"
#include "stores/ObjectStore.h"
#include "stores/CommitHistory.h"

namespace Split {

    ManualCommiter::ManualCommiter(const str& repoPath) : repoPath(repoPath) {}

    ManualCommiter::~ManualCommiter() {}

    void ManualCommiter::uploadTree(const str& treeHash, const str& treeContent) {
        const std::string objectPath = repoPath + "/.split/objects/trees/" + treeHash;

        std::ofstream outFile(objectPath, std::ios::binary);
        if (!outFile) {
            throw std::runtime_error("Failed to create object file: " + objectPath);
        }

        outFile << treeContent;
        outFile.close();
    }

    void ManualCommiter::uploadCommit(const str& commitHash, const str& commitContent) {
        const std::string objectPath = repoPath + "/.split/objects/commits/" + commitHash;

        std::ofstream outFile(objectPath, std::ios::binary);
        if (!outFile) {
            throw std::runtime_error("Failed to create object file: " + objectPath);
        }

        outFile << commitContent;
        outFile.close();
    }

}
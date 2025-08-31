//
// Created by Ahmed Mustafa on 8/13/2025.
//

#include "Repository.h"

#include <filesystem>
#include <utility>

namespace Split {

    Repository::Repository(const str &rootPath)
    : rootPath(rootPath), index(rootPath), history(rootPath) {
    }

    void Repository::init() const {
        const std::string repoPath = rootPath + "/.split";

        if (std::filesystem::exists(repoPath)) {
            throw std::runtime_error("Repository already initialized at " + repoPath);
        }

        std::filesystem::create_directories(repoPath);
    }

    void Repository::add(const std::string &filepath) {
        index.stageFile(filepath);
    }

    void Repository::commit(const std::string &message, const std::string &author) {
        Commit commit(rootPath, message, author, history.getLatest());
        commit.commit(index);
        const auto commitSerialized = commit.serialize();
        const str commitHash = ObjectStore(rootPath, "/commits").storeBytesObject(commitSerialized);
        history.addCommit(commitHash);
    }

    void Repository::checkout(const std::string &commitHash) {
        ObjectStore objectStore(rootPath, "/commits");
        auto commitStream = objectStore.loadObject(commitHash);
        if (!commitStream.is_open()) {
            throw std::runtime_error("Commit not found: " + commitHash);
        }

        Commit commit = Commit::deserialize(rootPath, commitStream);
        commitStream.close();

        commit.checkout(index);
    }

    std::vector<str> Repository::getCommitHistory() const {
        return history.getCommitHistory();
    }

}
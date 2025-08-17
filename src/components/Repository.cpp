//
// Created by Ahmed Mustafa on 8/13/2025.
//

#include "Repository.h"

#include <filesystem>
#include <utility>

namespace Split {

    Repository::Repository(std::string name, const std::string &rootPath)
    : name(std::move(name)), rootPath(rootPath), index(rootPath), history(rootPath) {
    }

    void Repository::init() const {

        // Initialize the repository by creating necessary directories and files
        const std::string repoPath = rootPath + "/.split";
        std::filesystem::create_directories(repoPath);
    }

    void Repository::add(const std::string &filepath) {
        index.stageFile(rootPath + "/" + filepath);
    }

    void Repository::commit(const std::string &message, const std::string &author) {
        Commit commit(rootPath, message, author, history.getLatest());
        commit.commit(index);
        const str commitHash = ObjectStore(rootPath, "/commits").storeBytesObject(commit.serialize());
        history.addCommit(commitHash);
    }

    void Repository::checkout(const std::string &commitHash) const {
        ObjectStore objectStore(rootPath, "/commits");
        auto commitStream = objectStore.loadObject(commitHash);
        if (!commitStream.is_open()) {
            throw std::runtime_error("Commit not found: " + commitHash);
        }

        Commit commit = Commit::deserialize(commitStream);
        commitStream.close();

        commit.checkout(index);
    }

}
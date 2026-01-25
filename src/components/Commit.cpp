//
// Created by Ahmed Mustafa on 8/16/2025.
//

#include "Commit.h"

#include <stores/ObjectStore.h>
#include <fstream>
#include <stdexcept>
#include <filesystem>

#include "Tree.h"
#include "stores/Pack.h"
#include "utils/Time.h"

namespace Split {

    Commit::Commit(const str &rootPath, const str &message, const str &author, const str &parentHash)
        : rootPath(rootPath), parentHash(parentHash), message(message), author(author)
    {}

    void Commit::commit(const Index &index) {
        const Tree tree(index.getStagedFiles());
        const ObjectStore store(rootPath, "/trees");
        treeHash = store.storeBytesObject(tree.serialize());
        timestamp = Time::getCurrentTime();
    }

    void Commit::checkout(Index &index) const {
        const ObjectStore treeStore(rootPath, "/trees");
        auto treeStream = treeStore.loadObject(treeHash);
        if (!treeStream.is_open()) {
            throw std::runtime_error("Tree not found: " + treeHash);
        }

        Tree tree = Tree::deserialize(treeStream);
        treeStream.close();

        const auto treeEntries = tree.getEntries();

        for (const auto& entry : treeEntries) {
            Pack pack(rootPath);

            IndexEntry indexEntry = {entry.first};

            indexEntry.baseVersionHash = pack.getBasePackByHash(entry.second).hash;
            indexEntry.blobHash = entry.second;
            indexEntry.isDeleted = false;
            index.updateEntry(entry.first, indexEntry);

            const auto entryPath = rootPath + "/" + entry.first;
            pack.decode(entry.second, entryPath);
        }

        const auto currentEntries = index.getEntries();

        // Delete files that are not in the commit
        for (const auto& [path, entry] : currentEntries) {
            if (treeEntries.find(path) == treeEntries.end() && !entry.isDeleted) {
                std::filesystem::remove(
                    std::filesystem::path(rootPath) / std::filesystem::path(path)
                );
                index.removeEntry(path);
            }
        }

        index.save();

    }

    str Commit::serialize() const {
        return treeHash + '\n'
            + parentHash + '\n'
            + message + '\n'
            + author + '\n'
            + timestamp;
    }

    Commit Commit::deserialize(const str& repoPath, std::istream &in) {
        str treeHash, parentHash, message, author, timestamp;
        std::getline(in, treeHash);
        std::getline(in, parentHash);
        std::getline(in, message);
        std::getline(in, author);
        std::getline(in, timestamp);

        Commit commit(repoPath, message, author, parentHash);
        commit.treeHash = treeHash;
        commit.timestamp = timestamp;

        return commit;
    }

}

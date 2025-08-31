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

        const auto currentEntries = index.getEntries();
        const auto treeEntries = tree.getEntries();

        for (const auto& entry : treeEntries) {
            Pack pack(rootPath);

            IndexEntry indexEntry = {};

            indexEntry.baseVersionHash = pack.getBaseVersionHash(entry.second);
            indexEntry.blobHash = entry.second;
            indexEntry.isDeleted = false;
            index.updateEntry(entry.first, indexEntry);

            auto decodedContent = pack.getDecodedContent(entry.second);

            std::ofstream fileStream(rootPath + "/" + entry.first, std::ios::binary);
            if (!fileStream.is_open()) {
                throw std::runtime_error("Failed to open file for writing: " + entry.first);
            }

            // If content is empty it's the base blob
            if (decodedContent == "\n") {
                ObjectStore blobStore(rootPath, "/blobs");
                auto baseBlobStream = blobStore.loadObject(indexEntry.baseVersionHash);
                if (!baseBlobStream.is_open()) {
                    throw std::runtime_error("Base blob not found: " + indexEntry.baseVersionHash);
                }
                std::ostringstream baseBlobStringStream;
                baseBlobStringStream << baseBlobStream.rdbuf();
                decodedContent = baseBlobStringStream.str();
            }

            fileStream << decodedContent;
            fileStream.close();

        }

        std::ofstream fileCheck(rootPath + "/" + "SPLIT_IS_CREATED", std::ios::binary);
        fileCheck << "This file indicates that the repository has been initialized by Split VCS.";
        fileCheck.close();


        // Delete files that are not in the commit
        for (const auto& indexEntry : currentEntries) {
            if (treeEntries.find(indexEntry.first) == treeEntries.end() && !indexEntry.second.isDeleted) {
                std::filesystem::remove(indexEntry.first);
                index.removeEntry(indexEntry.first);
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

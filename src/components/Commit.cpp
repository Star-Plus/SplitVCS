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

    void Commit::checkout(Index &index) {
        const ObjectStore treeStore(rootPath, "/trees");
        auto treeStream = treeStore.loadObject(treeHash);
        if (!treeStream.is_open()) {
            throw std::runtime_error("Tree not found: " + treeHash);
        }

        Tree tree = Tree::deserialize(treeStream);
        treeStream.close();

        const auto currentEntries = index.getEntries();

        for (const auto& entry : tree.getEntries()) {

            if (currentEntries.find(entry.first) == currentEntries.end()) {
                index.removeEntry(entry.first);
                std::filesystem::remove(entry.first);
            }
            else {
                IndexEntry indexEntry = currentEntries.at(entry.first);
                indexEntry.blobHash = entry.second;
                indexEntry.isDeleted = false;
                index.updateEntry(entry.first, indexEntry);

                Pack pack(rootPath);
                auto decodedContent = pack.getDecodedContent(entry.second);

                std::ofstream fileStream(entry.first, std::ios::binary);
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

                if (!decodedContent.empty()) {
                    fileStream << decodedContent;
                    fileStream.close();
                } else {
                    throw std::runtime_error("Failed to decode content for " + entry.first);
                }

            }
        }

    }

    str Commit::serialize() const {
        return rootPath + '\n'
            + treeHash + '\n'
            + parentHash + '\n'
            + message + '\0'
            + author + '\0'
            + timestamp;
    }

    Commit Commit::deserialize(std::istream &in) {
        str rootPath, treeHash, parentHash, message, author, timestamp;
        std::getline(in, rootPath);
        std::getline(in, treeHash);
        std::getline(in, parentHash);
        std::getline(in, message, '\0');
        std::getline(in, author, '\0');
        std::getline(in, timestamp);

        return {rootPath, message, author, parentHash};
    }

}

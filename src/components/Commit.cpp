//
// Created by Ahmed Mustafa on 8/16/2025.
//

#include "Commit.h"

#include <stores/ObjectStore.h>
#include <fstream>

#include "Tree.h"
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

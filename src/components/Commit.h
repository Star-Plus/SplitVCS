//
// Created by Ahmed Mustafa on 8/13/2025.
//

#ifndef COMMIT_H
#define COMMIT_H

#include <string>
#include <stores/Index.h>

#include "core/Alias.h"

namespace Split {

    class Commit {


    public:

        str rootPath;
        std::string treeHash;
        std::string parentHash;
        std::string message;
        std::string author;
        std::string timestamp;

        Commit(const str &rootPath, const str& message, const str& author, const str& parentHash = "");
        void commit (const Index& index);
        void checkout(Index& index) const;
        str serialize() const;
        static Commit deserialize(std::istream& in);

    };

}

#endif //COMMIT_H

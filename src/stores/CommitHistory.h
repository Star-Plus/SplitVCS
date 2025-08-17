//
// Created by Ahmed Mustafa on 8/13/2025.
//

#ifndef COMMITHISTORY_H
#define COMMITHISTORY_H

#include <vector>
#include <stdexcept>
#include <fstream>

#include "ObjectStore.h"
#include "../components/Commit.h"
#include "utils/Hashing.h"

namespace Split {

    class CommitHistory {

        str rootPath;
        str storePath = ".split/history";
        std::vector<str> commitHashes;

    public:

        CommitHistory(const str& rootPath)
            : rootPath(rootPath) {

            std::ifstream file(rootPath + "/" + storePath);
            if (!file.is_open()) {
                file.close();
                return;
            }
            std::string line;
            while (std::getline(file, line)) {
                if (!line.empty()) {
                    commitHashes.push_back(line);
                }
            }
        }

        void addCommit(const str& commit) {
            commitHashes.push_back(commit);
            if (!save()) {
                throw std::runtime_error("Failed to save commit history.");
            }
        }

        str getLatest() {
            if (commitHashes.empty()) {
                return "";
            }
            return commitHashes.back();
        }

        bool save() const {
            std::ofstream file(rootPath + "/" + storePath, std::ios::binary);
            if (!file.is_open()) {
                return false;
            }

            for (const auto& commit : commitHashes) {
                file << commit << '\n';
            }

            file.close();
            return true;
        }

    };

}

#endif //COMMITHISTORY_H

//
// Created by Ahmed Mustafa on 8/24/2025.
//

#include "RepositoryStatus.h"

#include <filesystem>
#include <components/Repository.h>

namespace Split {

    str RepositoryStatus::getRepoStatus(const str& repoPath) {
        const Repository repo(repoPath);
        const auto entries = repo.getIndex().getEntries();
        //
        // // Loop over repo path files
        str statusReport;
        for (const auto& entry : std::filesystem::recursive_directory_iterator(repoPath)) {
            // if (entry.is_directory()) continue;
            // if (entry.path().string().find(".split") != std::string::npos) continue;
        //
        //     const str relativePath = relative(entry.path(), repoPath).string();
        //     str fileStatus = relativePath + ":";
        //
        //     const auto entryIt = entries.find(relativePath);
        //     if (entryIt == entries.end()) {
        //         fileStatus += "Untracked";
        //     } else {
        //         const auto newHash = Hashing::computeFileHash(entry.path().string());
        //         if (newHash != entryIt->second.blobHash) {
        //             fileStatus += "Modified";
        //         } else {
        //             continue; // No changes
        //         }
        //     }
        //
        //     statusReport += fileStatus + "\n";
        // }
        //
        // // Check for deleted files
        // for (const auto& [filePath, entry] : entries) {
        //     if (!exists(std::filesystem::path(repoPath) / filePath)) {
        //         statusReport += filePath + ":Deleted\n";
        //     }
        }

        return statusReport.empty() ? "Clean" : statusReport;

    }

}

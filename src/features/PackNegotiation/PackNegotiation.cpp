//
// Created by Ahmed Mustafa on 8/26/2025.
//

#include <set>

#include "PackNegotiation.h"
#include "CommitDiffer.h"
#include "stores/Pack.h"


namespace Split {

    str PackNegotiation::negotiatePack(const str& requestedCommit) const {
        const auto commitHistory = repo.getCommitHistory();

        // Search for the requested commit in the history
        const auto it = std::ranges::find(commitHistory, requestedCommit);
        const auto itIdx = std::distance(commitHistory.begin(), it);

        const CommitDiffer differ(repo.getRootPath());

        std::set<str> packData;

        ObjectStore commitStore(repo.getRootPath(), "/commits");
        ObjectStore blobStore(repo.getRootPath(), "/blobs");
        ObjectStore deltaStore(repo.getRootPath(), "/deltas");

        Pack pack(repo.getRootPath());

        for (size_t i = itIdx+1; i < commitHistory.size(); ++i) {
            auto commitObj = commitStore.loadObject(commitHistory[i]);
            auto commit = Commit::deserialize(commitObj);

            packData.insert(".split/objects/commits/" + commitHistory[i]);
            packData.insert(".split/objects/trees/" + commit.treeHash);

            const auto diffs = differ.calculateDiffs(commitHistory[itIdx], commitHistory[i]);

            for (const auto& diff : diffs) {
                if (diff.type == DiffType::ADDED || diff.type == DiffType::MODIFIED) {
                    auto [hash, baseHash, deltaHash, baseRef] = pack.getPackUnitByHash(diff.hash);
                    if (baseHash.empty()) {
                        packData.insert(".split/objects/blobs/" + diff.hash);
                    }
                    else {
                        packData.insert(".split/objects/deltas/" + deltaHash);
                    }
                }
            }
        }

        // Push the history file
        packData.insert(".split/history");

        str parsedPackData;

        for (const auto& item : packData) {
            parsedPackData += item + "\n";
        }

        return parsedPackData;

    }

}

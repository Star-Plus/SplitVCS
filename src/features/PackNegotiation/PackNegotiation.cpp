//
// Created by Ahmed Mustafa on 8/26/2025.
//

#include <set>
#include <filesystem>

#include "PackNegotiation.h"
#include "CommitDiffer.h"
#include "stores/Pack.h"
#include "utils/Parsers/ParserToString.h"


namespace Split {

    str PackNegotiation::negotiatePack(const str& requestedCommit) const {

        std::set<str> packData;

        if (requestedCommit=="EMPTY") {
            auto repoDir = std::filesystem::recursive_directory_iterator(repo.getRootPath() + "/.split");
            for (const auto& entry : repoDir) {
                // ignore index file
                if (entry.path().stem() == "index") {
                    continue;
                }

                if (entry.is_regular_file()) {
                    packData.insert(entry.path().string().substr(repo.getRootPath().size()+1));
                }
            }

            return ParserToString::fromSet(packData);
        }

        const auto commitHistory = repo.getCommitHistory();

        // Search for the requested commit in the history
        const auto it = std::ranges::find(commitHistory, requestedCommit);
        const auto itIdx = std::distance(commitHistory.begin(), it);

        const CommitDiffer differ(repo.getRootPath());


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
                        packData.insert(".split/refs/packs/"+hash);
                        packData.insert(".split/objects/deltas/" + deltaHash);
                    }
                }
            }
        }

        // Push the history file
        packData.insert(".split/history");

        return ParserToString::fromSet(packData);

    }

}

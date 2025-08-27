//
// Created by Ahmed Mustafa on 8/18/2025.
//

#include "SplitInterface.h"

#include "components/Repository.h"
#include "features/RepositoryStatus.h"

namespace Split {

    void SplitInterface::Init(const str& rootPath) {
        const Repository repo(rootPath);
        repo.init();
    }

    void SplitInterface::Add(const str& rootPath, const str& filePath) {
        Repository repo(rootPath);
        repo.add(filePath);
    }

    void SplitInterface::Commit(const str& rootPath, const str& commitMessage, const str& author) {
        Repository repo(rootPath);
        repo.commit(commitMessage, author);
    }

    void SplitInterface::Checkout(const str& rootPath, const str& commitId) {
        Repository repo(rootPath);
        repo.checkout(commitId);
    }

    str SplitInterface::GetStatus(const str& rootPath) {
        return RepositoryStatus::getRepoStatus(rootPath);
    }

    std::vector<str> SplitInterface::GetCommitHistory(const str& rootPath) {
        const Repository repo(rootPath);
        return repo.getCommitHistory();
    }

}

//
// Created by Ahmed Mustafa on 8/18/2025.
//

#include "SplitInterface.h"

#include "components/Repository.h"
#include "features/RepositoryStatus.h"
#include "features/FastLiveAsset/TinyAsset.h"
#include "features/IgnoreList/IgnoreList.h"
#include "features/PackNegotiation/PackNegotiation.h"
#include "utils/Parsers/ParserToString.h"
#include "features/ManualCommit/ManualCommiter.h"

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

    str SplitInterface::Negotiate(const str& repoPath, const str& requestedCommitId) {
        const PackNegotiation negotiator(repoPath);
        return negotiator.negotiatePack(requestedCommitId);
    }

    str SplitInterface::GetStatus(const str& rootPath) {
        return RepositoryStatus::getRepoStatus(rootPath);
    }

    std::vector<str> SplitInterface::GetCommitHistory(const str& rootPath) {
        const Repository repo(rootPath);
        return repo.getCommitHistory();
    }

    str SplitInterface::GetCommitHistoryAsString(const str& rootPath) {
        const Repository repo(rootPath);
        const auto commits = repo.getCommitHistory();
        return ParserToString::fromVector(commits);
    }

    void SplitInterface::ManualUploadTree(const str& repoPath, const str& treeHash, const str& treeContent) {
        ManualCommiter commiter(repoPath);
        commiter.uploadTree(treeHash, treeContent);
    }

    void SplitInterface::ManualUploadCommit(const str& repoPath, const str& commitHash, const str& commitContent) {
        ManualCommiter commiter(repoPath);
        commiter.uploadCommit(commitHash, commitContent);
    }

    void SplitInterface::AddIgnore(const str& repoPath, const str& ignorePath)
    {
        IgnoreList il(repoPath);
        il.add(ignorePath);
    }

    void SplitInterface::RemoveIgnore(const str& repoPath, const str& ignorePath)
    {
        IgnoreList il(repoPath);
        il.remove(ignorePath);
    }

    bool SplitInterface::IsIgnored(const str& repoPath, const str& ignorePath)
    {
        const IgnoreList il(repoPath);
        return il.isIgnored(ignorePath);
    }

    str SplitInterface::GetTinyAsset(const str& repoPath, const str& versionHash, const str& fileType, int quality)
    {
        const TinyAsset ta(repoPath);
        return ta.encodeAsset(versionHash, fileType, quality);
    }
}

#include <SplitInterface.h>

extern "C" {

    void init(const char* rootPath) {
        Split::SplitInterface::Init(rootPath);
    }

    void add(const char* rootPath, const char* filePath) {
        Split::SplitInterface::Add(rootPath, filePath);
    }

    void commit(const char* rootPath, const char* message, const char* author) {
        Split::SplitInterface::Commit(rootPath, message, author);
    }

    void checkout(const char* rootPath, const char* commitId) {
        Split::SplitInterface::Checkout(rootPath, commitId);
    }

    const char* getCommitHistory(const char* rootPath) {
        static std::string s;
        s = Split::SplitInterface::GetCommitHistoryAsString(rootPath);
        return s.c_str();
    }

    const char* rstatus(const char* rootPath) {
        static std::string s;
        s = Split::SplitInterface::GetStatus(rootPath);
        return s.c_str();
    }

    const char* negotiate(const char* rootPath, const char* requestedCommitId) {
        static std::string s;
        s = Split::SplitInterface::Negotiate(rootPath, requestedCommitId);
        return s.c_str();
    }

    const void manualUploadTree(const char* repoPath, const char* treeHash, const char* treeContent) {
        Split::SplitInterface::ManualUploadTree(repoPath, treeHash, treeContent);
    }

    const void manualUploadCommit(const char* repoPath, const char* commitHash, const char* commitContent) {
        Split::SplitInterface::ManualUploadCommit(repoPath, commitHash, commitContent);
    }

}
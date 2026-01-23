//
// Created by Ahmed Mustafa on 1/23/2026.
//

#ifndef SPLITVCS_SPLIT_C_API_H
#define SPLITVCS_SPLIT_C_API_H

extern "C" {

    void init(const char* rootPath);

    void add(const char* rootPath, const char* filePath);

    void commit(const char* rootPath, const char* message, const char* author);

    void checkout(const char* rootPath, const char* commitId);

    const char* getCommitHistory(const char* rootPath);

    const char* rstatus(const char* rootPath);

    const char* negotiate(const char* rootPath, const char* requestedCommitId);

    void manualUploadTree(const char* repoPath, const char* treeHash, const char* treeContent);

    void manualUploadCommit(const char* repoPath, const char* commitHash, const char* commitContent);

    void addIgnore(const char* repoPath, const char* filePath);

    void removeIgnore(const char* repoPath, const char* filePath);

    bool isIgnored(const char* repoPath, const char* filePath);

    const char* getTinyAsset(const char* repoPath, const char* versionHash, int quality);

}

#endif //SPLITVCS_SPLIT_C_API_H
//
// Created by Ahmed Mustafa on 1/23/2026.
//

#ifndef SPLITVCS_SPLIT_C_API_H
#define SPLITVCS_SPLIT_C_API_H

#ifdef _WIN32
    #ifdef SPLIT_C_EXPORTS
        #define SPLIT_API __declspec(dllexport)
    #else
        #define SPLIT_API __declspec(dllimport)
    #endif
#else
    #define SPLIT_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

SPLIT_API void init(const char* rootPath);

SPLIT_API void add(const char* rootPath, const char* filePath);

SPLIT_API void commit(const char* rootPath, const char* message, const char* author);
SPLIT_API void checkout(const char* rootPath, const char* commitId);

SPLIT_API const char* getCommitHistory(const char* rootPath);

SPLIT_API const char* rstatus(const char* rootPath);
SPLIT_API const char* negotiate(const char* rootPath, const char* requestedCommitId);

SPLIT_API void manualUploadTree(const char* repoPath, const char* treeHash, const char* treeContent);

SPLIT_API void manualUploadCommit(const char* repoPath, const char* commitHash, const char* commitContent);
SPLIT_API void addIgnore(const char* repoPath, const char* filePath);

SPLIT_API void removeIgnore(const char* repoPath, const char* filePath);

SPLIT_API bool isIgnored(const char* repoPath, const char* filePath);
SPLIT_API const char* getTinyAsset(const char* repoPath, const char* versionHash, int quality);

#ifdef __cplusplus
}
#endif

#endif //SPLITVCS_SPLIT_C_API_H
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

    const char* status(const char* rootPath) {
        static std::string status;
        status = Split::SplitInterface::GetStatus(rootPath);
        return status.c_str();
    }

}
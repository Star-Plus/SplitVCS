//
// Created by Ahmed Mustafa on 8/24/2025.
//

#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <components/Repository.h>
#include <features/RepositoryStatus.h>

#include "SplitInterface.h"

TEST(RespositoryStatusTest, UntrackedFiles) {
    const str repoPath = "test_repo";
    std::filesystem::create_directory(repoPath);
    std::ofstream(repoPath + "/file1.txt") << "This is an untracked file.";
    std::ofstream(repoPath + "/file2.txt") << "This is another untracked file.";

    const auto status = Split::RepositoryStatus::getRepoStatus(repoPath);

    EXPECT_EQ(status, "file1.txt:Untracked\nfile2.txt:Untracked\n");

    std::filesystem::remove_all(repoPath);
}

TEST(RepositoryStatusTest, UnmodifiedFiles) {
    const str repoPath = "test_repo";
    std::filesystem::create_directory(repoPath);
    std::ofstream(repoPath + "/file1.txt") << "This is an untracked file.";
    std::ofstream(repoPath + "/file2.txt") << "This is another untracked file.";

    Split::SplitInterface::Init(repoPath);
    Split::SplitInterface::Add(repoPath, "file1.txt");
    Split::SplitInterface::Add(repoPath, "file2.txt");
    Split::SplitInterface::Commit(repoPath, "Initial commit", "Tester");

    const auto status = Split::RepositoryStatus::getRepoStatus(repoPath);
    EXPECT_EQ(status, "Clean");

    std::filesystem::remove_all(repoPath);
}

TEST(RepositoryStatusTest, ModifiedFiles) {
    const str repoPath = "test_repo";
    std::filesystem::create_directory(repoPath);
    std::ofstream(repoPath + "/file1.txt") << "This is an untracked file.";
    std::ofstream(repoPath + "/file2.txt") << "This is another untracked file.";

    Split::SplitInterface::Init(repoPath);
    Split::SplitInterface::Add(repoPath, "file1.txt");
    Split::SplitInterface::Add(repoPath, "file2.txt");
    Split::SplitInterface::Commit(repoPath, "Initial commit", "Tester");

    std::ofstream(repoPath + "/file1.txt") << "This is a modified file.";
    std::ofstream(repoPath + "/file2.txt") << "This is another modified file.";

    const auto status = Split::RepositoryStatus::getRepoStatus(repoPath);
    EXPECT_EQ(status, "file1.txt:Modified\nfile2.txt:Modified\n");

    std::filesystem::remove_all(repoPath);
}

TEST(RepositoryStatusTest, DeletedFiles) {
    const str repoPath = "test_repo";
    std::filesystem::create_directory(repoPath);
    std::ofstream(repoPath + "/file1.txt") << "This is an untracked file.";
    std::ofstream(repoPath + "/file2.txt") << "This is another untracked file.";

    Split::SplitInterface::Init(repoPath);
    Split::SplitInterface::Add(repoPath, "file1.txt");
    Split::SplitInterface::Add(repoPath, "file2.txt");
    Split::SplitInterface::Commit(repoPath, "Initial commit", "Tester");

    remove((repoPath + "/file1.txt").c_str());
    remove((repoPath + "/file2.txt").c_str());

    const auto status = Split::RepositoryStatus::getRepoStatus(repoPath);
    EXPECT_EQ(status, "file1.txt:Deleted\nfile2.txt:Deleted\n");

    std::filesystem::remove_all(repoPath);
}
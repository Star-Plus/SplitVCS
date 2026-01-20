//
// Created by Ahmed Mustafa on 8/31/2025.
//

#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include "SplitInterface.h"
#include "components/Repository.h"

TEST(CheckoutTest, EmptyRepo) {
    const str repoPath = "test_repo";
    std::filesystem::create_directory(repoPath);
    std::ofstream(repoPath + "/file1.txt") << "";

    Split::SplitInterface::Init(repoPath);
    Split::SplitInterface::Add(repoPath, "file1.txt");
    Split::SplitInterface::Commit(repoPath, "Initial commit", "Tester");

    std::remove((repoPath + "/file1.txt").c_str());
    std::remove((repoPath + "/.split/index").c_str());

    const auto commitHistory = Split::SplitInterface::GetCommitHistory(repoPath);

    Split::SplitInterface::Checkout(repoPath, commitHistory.back());

    std::filesystem::remove_all(repoPath);
}

TEST(CheckoutTest, NonEmptyRepo) {
    const str repoPath = "test_repo_non_empty";
    std::filesystem::create_directory(repoPath);
    std::ofstream(repoPath + "/file1.txt") << "Hello, World!";
    std::ofstream(repoPath + "/file2.txt") << "Goodbye, World!";

    Split::Repository repo(repoPath);

    repo.init();
    repo.add("file1.txt");
    repo.commit("Add file1", "Tester");

    const auto commitHistory = repo.getCommitHistory();
    repo.checkout(commitHistory.back());

    std::filesystem::path file1Path = repoPath + "/file1.txt";
    EXPECT_TRUE(std::filesystem::exists(file1Path));

    const auto entries = repo.getIndex().getEntries();

    EXPECT_FALSE(entries.at("file1.txt").isDeleted);
    EXPECT_TRUE(entries.find("file1.txt") != entries.end());

    std::filesystem::remove_all(repoPath);
}
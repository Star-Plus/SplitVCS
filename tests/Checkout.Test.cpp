//
// Created by Ahmed Mustafa on 8/31/2025.
//

#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include "SplitInterface.h"

TEST(CheckoutTest, EmptyRepo) {
    const str repoPath = "test_repo";
    std::filesystem::create_directory(repoPath);
    std::ofstream(repoPath + "/file1.txt") << "This is an untracked file.";

    Split::SplitInterface::Init(repoPath);
    Split::SplitInterface::Add(repoPath, "file1.txt");
    Split::SplitInterface::Commit(repoPath, "Initial commit", "Tester");

    std::remove((repoPath + "/file1.txt").c_str());
    // std::remove((repoPath + "/.split/index").c_str());

    const auto commitHistory = Split::SplitInterface::GetCommitHistory(repoPath);

    Split::SplitInterface::Checkout(repoPath, commitHistory.back());

    std::filesystem::remove_all(repoPath);
}
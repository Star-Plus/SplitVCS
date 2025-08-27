//
// Created by Ahmed Mustafa on 8/26/2025.
//

#include <gtest/gtest.h>
#include <filesystem>

#include "SplitInterface.h"
#include "features/PackNegotiation/CommitDiffer.h"

TEST(CommitDifferTest, CalculateDiffers) {

    const str rootPath = "test_repo";
    std::filesystem::create_directory(rootPath);

    Split::SplitInterface::Init(rootPath);

    std::fstream file1(rootPath + "/file1.txt", std::ios::out);
    file1 << "Hello, World!" << std::endl;
    file1.close();

    Split::SplitInterface::Add(rootPath, "file1.txt");
    Split::SplitInterface::Commit(rootPath, "Initial commit", "Tester");

    file1.open(rootPath + "/file1.txt", std::ios::out | std::ios::app);
    file1 << "This is a test file." << std::endl;
    file1.close();

    Split::SplitInterface::Add(rootPath, "file1.txt");
    Split::SplitInterface::Commit(rootPath, "Updated file1.txt", "Tester");

    const auto commits = Split::SplitInterface::GetCommitHistory(rootPath);

    ASSERT_EQ(commits.size(), 2);

    Split::CommitDiffer differ(rootPath);
    const auto diffs = differ.calculateDiffs(commits[0], commits[1]);
    ASSERT_EQ(diffs.size(), 1);

    ASSERT_EQ(diffs[0].filePath, "file1.txt");
    ASSERT_EQ(diffs[0].type, Split::DiffType::MODIFIED);

    std::filesystem::remove_all(rootPath);

}
//
// Created by Ahmed Mustafa on 8/27/2025.
//

#include <gtest/gtest.h>
#include <filesystem>

#include "SplitInterface.h"
#include "features/PackNegotiation/PackNegotiation.h"

TEST(PackNegotiationTest, BasicTest) {
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

    Split::PackNegotiation negotiator(rootPath);
    const auto packData = negotiator.negotiatePack(commits[0]);
    ASSERT_FALSE(packData.empty());

    std::cout << "Pack Data:\n" << packData << std::endl;

    std::filesystem::remove_all(rootPath);
}
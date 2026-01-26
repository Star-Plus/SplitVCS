//
// Created by Ahmed Mustafa on 8/27/2025.
//

#include <gtest/gtest.h>
#include <filesystem>

#include "SplitInterface.h"
#include "features/PackNegotiation/PackNegotiation.h"

TEST(PackNegotiationTest, BasicTest) {
    const str rootPath = "test_repo";

    std::filesystem::remove_all(rootPath);

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
}

TEST(PackNegotiationTest, ThreeCommits) {
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

    file1.open(rootPath + "/file1.txt", std::ios::out | std::ios::app);
    file1 << "Adding another line." << std::endl;
    file1.close();

    std::fstream file2(rootPath + "/file2.txt", std::ios::out);
    file2 << "This is file 2." << std::endl;
    file2.close();

    Split::SplitInterface::Add(rootPath, "file1.txt");
    Split::SplitInterface::Add(rootPath, "file2.txt");
    Split::SplitInterface::Commit(rootPath, "Added file2.txt and updated file1.txt", "Tester");

    const auto commits = Split::SplitInterface::GetCommitHistory(rootPath);

    ASSERT_EQ(commits.size(), 3);

    Split::PackNegotiation negotiator(rootPath);
    const auto packData = negotiator.negotiatePack(commits[1]);
    ASSERT_FALSE(packData.empty());

    std::cout << "Pack Data:\n" << packData << std::endl;

    std::filesystem::remove_all(rootPath);
}

TEST(PackNegotiationTest, EmptyRepo) {
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
    const auto packData = negotiator.negotiatePack("EMPTY");;
    ASSERT_FALSE(packData.empty());

    std::cout << "Pack Data:\n" << packData << std::endl;

    std::filesystem::remove_all(rootPath);
}
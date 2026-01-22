//
// Created by Ahmed Mustafa on 8/15/2025.
//

#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include <stores/Index.h>

#include "utils/Hashing.h"

TEST(IndexTest, StageCreatedFile) {
    const std::string rootPath = "test_repo";
    Split::Index index(rootPath);

    std::filesystem::create_directories(rootPath+ "/.split");

    std::string filePath = "test_file.txt";
    std::ofstream file(rootPath+"/"+filePath);
    ASSERT_TRUE(file.is_open());
    file << "Test content for staging.";
    file.close();

    index.stageFile(filePath);

    std::ifstream indexFile(rootPath + "/.split/index");
    ASSERT_TRUE(indexFile.is_open());
    indexFile.close();

    const std::string hash = Split::Hashing::computeFileHash(rootPath+"/"+filePath);

    std::ifstream objectFile(rootPath + "/.split/objects/blobs/" + hash);
    ASSERT_TRUE(objectFile.is_open());

    std::stringstream oss;
    oss << objectFile.rdbuf();

    ASSERT_EQ(oss.str(), "Test content for staging.");

    objectFile.close();

    const auto stagedEntries = index.getStagedFiles();
    ASSERT_EQ(stagedEntries.size(), 1);
    ASSERT_EQ(stagedEntries.begin()->first, filePath);
    ASSERT_EQ(stagedEntries.begin()->second, hash);
    // Clear the test environment
    std::filesystem::remove_all("test_repo");
}

TEST(IndexTest, StageModified) {
    const std::string rootPath = "test_repo";
    Split::Index index(rootPath);

    std::filesystem::create_directories(rootPath+ "/.split");

    std::string filePath = "/test_file.txt";
    std::ofstream file(rootPath+filePath);
    ASSERT_TRUE(file.is_open());
    file << "Test content for staging. Make it longer make it longer make it longer make it longer make it longer make it longer make it longer make it longer make it longer make it longer make it longer make it longer make it longer. Long long long long";
    file.close();

    index.stageFile(filePath);

    std::ifstream indexFile(rootPath + "/.split/index");
    ASSERT_TRUE(indexFile.is_open());
    indexFile.close();

    const std::string hash = Split::Hashing::computeFileHash(rootPath+filePath);

    std::ifstream objectFile(rootPath + "/.split/objects/blobs/" + hash);
    ASSERT_TRUE(objectFile.is_open());
    objectFile.close();

    // Modify the file
    file.open(rootPath+filePath, std::ios::out | std::ios::trunc);
    ASSERT_TRUE(file.is_open());
    file << "Modified content for staging. Make it longer make it longer make it longer make it longer make it longer make it longer make it longer make it longer make it longer make it longer make it longer make it longer make it longer. Long long long long";
    file.close();

    index.stageFile(filePath);
    std::ifstream modifiedIndexFile(rootPath + "/.split/index");
    ASSERT_TRUE(modifiedIndexFile.is_open());
    modifiedIndexFile.close();

    const std::string modifiedHash = Split::Hashing::computeFileHash(rootPath+filePath);

    ASSERT_EQ(index.getStagedFiles()[filePath], modifiedHash);

    std::ifstream packFile(rootPath + "/.split/refs/packs/" + modifiedHash +".pack");
    ASSERT_TRUE(packFile.is_open());
    packFile.close();

    // Clear the test environment
    std::filesystem::remove_all("test_repo");
}

TEST(IndexTest, StageUnmodified) {
    const std::string rootPath = "test_repo";
    Split::Index index(rootPath);

    std::filesystem::create_directories(rootPath+ "/.split");

    std::string filePath = "/test_file.txt";
    std::ofstream file(rootPath+filePath);
    ASSERT_TRUE(file.is_open());
    file << "Test content for staging.";
    file.close();

    index.stageFile(filePath);

    std::ifstream indexFile(rootPath + "/.split/index");
    ASSERT_TRUE(indexFile.is_open());
    indexFile.close();

    const std::string hash = Split::Hashing::computeFileHash(rootPath+filePath);

    std::ifstream objectFile(rootPath + "/.split/objects/blobs/" + hash);
    ASSERT_TRUE(objectFile.is_open());
    objectFile.close();

    const auto stagedEntries = index.getStagedFiles();
    ASSERT_EQ(stagedEntries.size(), 1);
    ASSERT_EQ(stagedEntries.begin()->first, filePath);
    ASSERT_EQ(stagedEntries.begin()->second, hash);

    index.stageFile(filePath);
    std::ifstream reindexFile(rootPath + "/.split/index");
    ASSERT_TRUE(reindexFile.is_open());
    reindexFile.close();
    const auto reStagedEntries = index.getStagedFiles();
    ASSERT_EQ(reStagedEntries.size(), 1);
    ASSERT_EQ(reStagedEntries.begin()->first, filePath);
    ASSERT_EQ(reStagedEntries.begin()->second, hash);

    // Clear the test environment
    std::filesystem::remove_all("test_repo");
}
//
// Created by Ahmed Mustafa on 8/15/2025.
//;

#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include <stores/ObjectStore.h>

#include <stores/Pack.h>

TEST(PackTest, EncodeDecode) {
    std::string rootPath = "test_root";
    std::filesystem::create_directories(rootPath + "/.split");

    Split::Pack pack(rootPath);

    std::string originalContent = "This is a test content for encoding and decoding.";
    std::string baseContent = "This is the base content.";

    Split::ObjectStore blobsObjectStore(rootPath, "/blobs");

    auto originalHash = blobsObjectStore.storeBytesObject(originalContent);

    auto original = blobsObjectStore.loadObject(originalHash);

    std::fstream base(rootPath + "/file.txt");

    // Encode the delta
    auto baseHash = pack.encodeBase(base);
    std::string encodedDelta = pack.encodeDelta(original, baseHash, originalHash);

    // Decode the content
    std::stringstream ss;
    pack.decode(originalHash, ss);

    // Check if the decoded content matches the original content
    EXPECT_EQ(ss.rdbuf()->str(), originalContent);
    std::cout << ss.rdbuf()->str() << std::endl;

    base.close();
    original.close();

    // Clean up files
    std::filesystem::remove_all(rootPath);
}

TEST(PackTest, EncodeMultipleTimes) {
    std::string rootPath = "test_root";
    std::filesystem::create_directories(rootPath + "/.split");

    Split::Pack pack(rootPath);

    std::string targetContent1 = "This is a test content for encoding and decoding.";
    std::string targetContent2 = "This is another test content for encoding and decoding.";
    std::string baseContent = "This is the base content.";

    Split::ObjectStore blobsObjectStore(rootPath, "/blobs");

    auto baseHash = blobsObjectStore.storeBytesObject(baseContent);
    auto targetHash1 = blobsObjectStore.storeBytesObject(targetContent1);
    auto targetHash2 = blobsObjectStore.storeBytesObject(targetContent2);

    auto base = blobsObjectStore.loadObject(baseHash);
    auto t1 = blobsObjectStore.loadObject(targetHash1);
    auto t2 = blobsObjectStore.loadObject(targetHash2);



    // Encode the delta
    std::string encodedDelta = pack.encodeDelta(t1, baseHash, targetHash1);
    // Encode again with a different target content
    std::string encodedDelta2 = pack.encodeDelta(t2, targetHash1, targetHash2);

    // Decode the content

    std::stringstream os1, os2;

    pack.decode(targetHash1, os1);
    pack.decode(targetHash2, os2);

    // Check if the decoded content matches the original content
    EXPECT_EQ(os1.rdbuf()->str(), targetContent1);
    EXPECT_EQ(os2.rdbuf()->str(), targetContent2);

    base.close();
    t1.close();
    t2.close();

    // Clean up files
    std::filesystem::remove_all(rootPath);
}

TEST(PackTest, EncodeBase) {
    std::string rootPath = "test_root";
    std::filesystem::create_directories(rootPath + "/.split");

    Split::Pack pack(rootPath);

    std::string baseContent = "This is the base content.";

    std::string originalPath = rootPath + "/file.txt";
    std::ofstream originalFile(originalPath, std::ios::binary);
    originalFile << baseContent;
    originalFile.close();

    Split::ObjectStore blobsObjectStore(rootPath, "/blobs");

    std::fstream base(rootPath + "/file.txt");
    base << baseContent;
    base.close();

    auto baseHash = pack.encodeBase(base);

    // Decode the content
    std::stringstream ss;
    pack.decode(baseHash, ss);

    // Check if the decoded content matches the original content
    EXPECT_EQ(ss.rdbuf()->str(), baseContent);
    std::cout << ss.rdbuf()->str() << std::endl;

    base.close();

    // Clean up files
    std::filesystem::remove_all(rootPath);
}
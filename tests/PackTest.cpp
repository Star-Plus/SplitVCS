//
// Created by Ahmed Mustafa on 8/15/2025.
//

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

    auto baseHash = blobsObjectStore.storeBytesObject(baseContent);
    auto originalHash = blobsObjectStore.storeBytesObject(originalContent);

    auto base = blobsObjectStore.loadObject(baseHash);
    auto original = blobsObjectStore.loadObject(originalHash);

    // Encode the delta
    std::string encodedDelta = pack.encodeDelta(base, original, baseHash, originalHash);

    // Decode the content
    std::stringstream ss;
    pack.decode(originalHash, ss);

    // Check if the decoded content matches the original content
    EXPECT_EQ(ss.rdbuf()->str(), originalContent);

    // Clean up files
    std::filesystem::remove_all(rootPath);
}

// TEST(PackTest, EncodeMultipleTimes) {
//     std::string rootPath = "test_root";
//     std::filesystem::create_directories(rootPath + "/.split");
//
//     Split::Pack pack(rootPath);
//
//     std::string targetContent1 = "This is a test content for encoding and decoding.";
//     std::string targetContent2 = "This is another test content for encoding and decoding.";
//     std::string baseContent = "This is the base content.";
//
//     Split::ObjectStore blobsObjectStore(rootPath, "/blobs");
//
//     auto baseHash = blobsObjectStore.storeBytesObject(baseContent);
//     auto targetHash1 = blobsObjectStore.storeBytesObject(targetContent1);
//     auto targetHash2 = blobsObjectStore.storeBytesObject(targetContent2);
//
//
//     // Encode the delta
//     std::string encodedDelta = pack.encodeDelta(baseContent, targetContent1, baseHash, targetHash1);
//     // Encode again with a different target content
//     std::string encodedDelta2 = pack.encodeDelta(targetContent1, targetContent2, targetHash1, targetHash2);
//
//     // Decode the content
//     std::string decodedContent1 = pack.getDecodedContent(targetHash1);
//     std::string decodedContent2 = pack.getDecodedContent(targetHash2);
//
//     // Check if the decoded content matches the original content
//     EXPECT_EQ(decodedContent1, targetContent1);
//     EXPECT_EQ(decodedContent2, targetContent2);
//
//     // Clean up files
//     std::filesystem::remove_all(rootPath);
// }
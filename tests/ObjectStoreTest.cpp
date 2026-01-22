#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "stores/ObjectStore.h"

TEST(ObjectStoreTest, StoreFiles) {
    const std::string rootPath = "test_object_store";
    Split::ObjectStore store(rootPath);
    const std::string filePath = "test_file.txt";
    const std::string objectPath = rootPath + "/.split/objects";

    std::fstream file(rootPath + "/" + filePath, std::ios::out);
    ASSERT_TRUE(file.is_open()) << "Failed to create test file for storing";
    file << "This is a test file for ObjectStore.";
    file.close();
    const auto hash = store.storeFileObject(filePath);
    ASSERT_FALSE(hash.empty()) << "Hash should not be empty after storing file";
    ASSERT_TRUE(store.hasObject(hash)) << "Object should be stored successfully";

    std::fstream object (objectPath+"/"+hash, std::ios::in);
    ASSERT_TRUE(object.is_open()) << "Stored object file should be accessible";
    object.close();

    // Delete test file and object store
    std::filesystem::remove_all(rootPath);
}
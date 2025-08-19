#include <iostream>
#include <components/Repository.h>


int main() {
    std::cout << "Hello, Split Version Control System!" << std::endl;

    Split::Repository repo("test_repo");
    repo.init();

    // Simulate adding a file and committing
    std::ofstream file("test_repo/file.txt");
    file << "This is a test file." << std::endl;
    file.close();

    repo.add("file.txt");
    repo.commit("Initial commit", "Author Name");

    // Modify the file
    file.open("test_repo/file.txt", std::ios::app);
    file << "Adding more content." << std::endl;
    file.close();

    repo.add("file.txt");
    repo.commit("Updated file.txt", "Author Name");

    auto history = repo.getCommitHistory();

    repo.checkout(history[0]);

    return 0;
}

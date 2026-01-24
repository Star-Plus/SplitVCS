#include <iostream>
#include <fstream>
#include "components/DeltaCompressor.h"

int main() {

    const auto Base = "OUR_CEO.png";
    const auto Modified = "OUR_INTERN.png";
    const auto Delta = "delta";
    const auto Constructed = "constructed.png";

    std::ifstream fileA(Base, std::ios::binary);
    if (!fileA) {
        std::cerr << "Failed to open " << Base << std::endl;
        return 1;
    }

    std::ifstream fileB(Modified, std::ios::binary);
    if (!fileB) {
        std::cerr << "Failed to open " << Modified << std::endl;
        return 1;
    }

    std::ofstream outputFile(Delta, std::ios::binary);
    if (!outputFile) {
        std::cerr << "Failed to create output " << Delta << std::endl;
        return 1;
    }

    Split::DeltaCompressor compressor;
    compressor.encode(
        Split::Blob(fileA, Split::AssetType::BINARY),
        Split::Blob(fileB, Split::AssetType::BINARY),
        Split::Blob(outputFile, Split::AssetType::BINARY)
    );

    std::ofstream constructedOutput(Constructed, std::ios::binary);
    if (!constructedOutput) {
        std::cerr << "Failed to create " << Constructed << std::endl;
        return 1;
    }

    fileA.close();
    fileB.close();
    outputFile.close();

    std::ifstream baseFile(Base, std::ios::binary);
    if (!baseFile) {
        std::cerr << "Failed to open " << Base << std::endl;
    }

    std::ifstream diffFile(Delta, std::ios::binary);
    if (!diffFile) {
        std::cerr << "Failed to open " << Delta << std::endl;
        return 1;
    }

    std::stack<std::unique_ptr<Split::Blob>> deltaStack;
    const auto delta = std::make_unique<Split::Blob>(diffFile, Split::AssetType::BINARY);
    deltaStack.push(delta);

    compressor.decode(
        Split::Blob(baseFile),
        deltaStack,
        Split::Blob(constructedOutput)
    );

    return 0;
}
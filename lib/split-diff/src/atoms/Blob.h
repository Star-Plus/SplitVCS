#include <istream>
#include <variant>
#include <string>
#include "enums/AssetType.h"

namespace Split {
    struct Blob {

        std::variant<std::istream*, std::ostream*> data;
        size_t size;
        std::string path;

        Blob(std::istream& inputStream, const std::string& assetPath)
            : data(&inputStream), size(calculateStreamSize()), path(assetPath) {}

        Blob(std::ostream& outputStream, const std::string& assetPath)
            : data(&outputStream), size(calculateStreamSize()), path(assetPath) {}

        Blob(std::istream* inputStream)
            : data(inputStream), size(calculateStreamSize()), path("") {
        }

        Blob(std::ostream* outputStream)
            : data(outputStream), size(calculateStreamSize()), path("") {
        }

        std::istream* getInputStream() const {
            if (std::holds_alternative<std::istream*>(data)) {
                return std::get<std::istream*>(data);
            }
            return nullptr;
        }

        std::ostream* getOutputStream() const {
            if (std::holds_alternative<std::ostream*>(data)) {
                return std::get<std::ostream*>(data);
            }
            return nullptr;
        }

        size_t calculateStreamSize() {
            if (std::holds_alternative<std::istream*>(data)) {
                
                std::istream* is = std::get<std::istream*>(data);
                std::streampos currentPos = is->tellg();
                is->seekg(0, std::ios::end);
                size_t dataSize = static_cast<size_t>(is->tellg());
                is->seekg(currentPos);
                return dataSize;
            }
            else {
                std::ostream* os = std::get<std::ostream*>(data);
                std::streampos currentPos = os->tellp();
                os->seekp(0, std::ios::end);
                size_t dataSize = static_cast<size_t>(os->tellp());
                os->seekp(currentPos);
                return dataSize;
            }

            return 0;
        }
    };
}
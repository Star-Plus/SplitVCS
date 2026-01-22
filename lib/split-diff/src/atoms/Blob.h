#pragma once

#include <istream>
#include <variant>
#include <string>

namespace Split {
    struct Blob {

        std::variant<std::istream*, std::ostream*> data;
        size_t size;
        AssetType type;

        Blob(std::istream& inputStream, AssetType type)
            : data(&inputStream), size(calculateStreamSize()), type(type) {}

        Blob(std::ostream& outputStream, AssetType type)
            : data(&outputStream), size(calculateStreamSize()), type(type) {}

        Blob(std::istream& inputStream)
            : data(&inputStream), size(calculateStreamSize()), type(AssetType::BINARY) {
        }

        Blob(std::ostream& outputStream)
            : data(&outputStream), size(calculateStreamSize()), type(AssetType::BINARY) {
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

        size_t calculateStreamSize() const
        {
            if (std::holds_alternative<std::istream*>(data)) {

                const auto is = std::get<std::istream*>(data);
                std::streampos currentPos = is->tellg();
                is->seekg(0, std::ios::end);
                size_t dataSize = is->tellg();
                is->seekg(currentPos);
                return dataSize;
            }

            std::ostream* os = std::get<std::ostream*>(data);
            std::streampos currentPos = os->tellp();
            os->seekp(0, std::ios::end);
            size_t dataSize = os->tellp();
            os->seekp(currentPos);
            return dataSize;
        }
    };
}
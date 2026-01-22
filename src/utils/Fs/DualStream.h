//
// Created by Ahmed Mustafa on 1/22/2026.
//

#ifndef SPLITVCS_DUALSTREAM_H
#define SPLITVCS_DUALSTREAM_H

#include <variant>
#include <fstream>
#include <sstream>

namespace Split
{
    class DualStream {
        std::variant<std::fstream, std::stringstream> stream;

    public:
        template<typename... Args>
        void emplaceFile(Args&&... args) {
            stream.emplace<std::fstream>(std::forward<Args>(args)...);
        }

        void emplaceMemory() {
            stream.emplace<std::stringstream>();
        }

        std::istream& in() {
            return std::visit([](auto& s) -> std::istream& { return s; }, stream);
        }

        std::ostream& out() {
            return std::visit([](auto& s) -> std::ostream& { return s; }, stream);
        }
    };

}

#endif //SPLITVCS_DUALSTREAM_H
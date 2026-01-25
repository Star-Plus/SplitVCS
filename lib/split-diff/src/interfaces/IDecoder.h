#pragma once
#include <ostream>
#include <stack>

namespace Split {

    class Decoder {
    public:
        virtual ~Decoder() = default;

        virtual void decode(const std::istream& base, const std::istream& delta, std::ostream& out) = 0;
        virtual void decode(const std::string& base, std::stack<std::string>& deltas, std::string& out) = 0;

    };

}
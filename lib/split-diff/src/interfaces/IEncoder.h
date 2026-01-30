#pragma once
#include <stack>
#include <istream>

namespace Split {

    class Encoder {
    public:
        virtual ~Encoder() = default;

        virtual void encode(const std::istream& v1, const std::istream& v2, std::ostream& out) = 0;

        virtual std::string encode(const std::string& base, const std::string& out) { return ""; }
        virtual std::string encode(const std::string& base, std::stack<std::string>& deltas, const std::string& v2, std::string& out) = 0;
    };

}
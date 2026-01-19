#pragma once
#include <ostream>

namespace Split {

    class Decoder {
    public:
        virtual ~Decoder() = default;

        virtual void decode(const std::istream& base, const std::istream& delta, std::ostream& out) = 0;
    };

}
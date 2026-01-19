#include <istream>
#include <ostream>

namespace Split {

    class Encoder {
    public:
        virtual void encode(const std::istream& v1, const std::istream& v2, std::ostream& out) = 0;
    };

}
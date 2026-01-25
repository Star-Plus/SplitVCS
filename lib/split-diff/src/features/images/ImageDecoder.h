#pragma once
#include "interfaces/IDecoder.h"
#include "main/Logger.h"
#include "opencv2/core/mat.hpp"

namespace Split {

    class ImageDecoder final : public Decoder {
    public:
        void decode(const std::istream& base, const std::istream& delta, std::ostream& out) override;
        void decode(const std::string& base, std::stack<std::string>& deltas, std::string& out) override;
        static cv::Mat decode(const std::string& base, std::stack<std::string>& deltas);
    private:
        Logger logger;
    };

}

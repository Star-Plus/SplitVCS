//
// Created by lightum on 1/19/26.
//

#pragma once

#include <opencv2/opencv.hpp>
#include <istream>

namespace Split {

    class ReadAdapter {
    public:
        static cv::Mat fastReadImage(std::istream& is, const int flags) {
            is.seekg(0, std::ios::end);
            const std::streamsize sz = is.tellg();
            is.seekg(0, std::ios::beg);

            if (sz <= 0) {
                return cv::Mat();
            }

            std::vector<uchar> buffer(static_cast<size_t>(sz));
            is.read(reinterpret_cast<char*>(buffer.data()), sz);
            return cv::imdecode(buffer, flags);
        }
    };

}
//
// Created by lightum on 1/20/26.
//

#ifndef SPLITVCS_PIXELCALCULATOR_H
#define SPLITVCS_PIXELCALCULATOR_H
#include <opencv2/opencv.hpp>

namespace Split {

    namespace PixelCalculator {

        void clockDifference(const cv::Mat& A, const cv::Mat& B, cv::Mat& C);
        void clockAdder(const cv::Mat& A, const cv::Mat& B, cv::Mat& C);

    }

}

#endif //SPLITVCS_PIXELCALCULATOR_H
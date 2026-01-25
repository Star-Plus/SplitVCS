//
// Created by lightum on 1/20/26.
//

#include "PixelCalculator.h"

#include "main/Logger.h"

namespace Split {

    namespace PixelCalculator {

        void clockDifference(const cv::Mat &A, const cv::Mat &B, cv::Mat &C) {
            Logger logger(true, "Clock Difference Calculator");


            logger.debug("Start calculating");

            for (size_t i = 0; i < A.rows; i++) {
                for (size_t j = 0; j < A.cols; j++) {
                    for (short k = 0; k < 3; k++) {
                        const int diff = static_cast<int>(B.at<cv::Vec3b>(i, j)[k]) - A.at<cv::Vec3b>(i, j)[k];
                        if (diff < 0) {
                            C.at<cv::Vec3b>(i, j)[k] = diff + 256;
                        } else {
                            C.at<cv::Vec3b>(i, j)[k] = diff;
                        }
                    }
                }
            }

            logger.debug("Done calculating");
        }

        void clockAdder(const cv::Mat &A, const cv::Mat &B, cv::Mat &C) {
            for (size_t i = 0; i < A.rows; i++) {
                for (size_t j = 0; j < A.cols; j++) {
                    for (short k = 0; k < 3; k++) {
                        const int dK = static_cast<int>(A.at<cv::Vec3b>(i, j)[k]) + B.at<cv::Vec3b>(i, j)[k];
                        if (dK < 255) {
                            C.at<cv::Vec3b>(i, j)[k] = dK - 256;
                        } else {
                            C.at<cv::Vec3b>(i, j)[k] = dK;
                        }
                    }
                }
            }
        }


    }

}


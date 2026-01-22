//
// Created by lightum on 1/19/26.
//

#include <opencv2/opencv.hpp>
#include <set>

namespace Split {
    namespace ImageDebugUtils {

        int calculateDistinctValuesCount(cv::Mat image) {
            std::set<int> distinctValues;

            for (int i = 0; i < image.cols; i++) {
                for (int j = 0; j < image.rows; j++) {
                    distinctValues.insert(image.at<uchar>(i, j));
                }
            }

            return distinctValues.size();
        }

    }
}
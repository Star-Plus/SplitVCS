#include <iostream>
#include <opencv2/opencv.hpp>

int main0() {

    cv::Mat sample1 = cv::imread("Samples/Split_Sample1.png", cv::IMREAD_COLOR);
    cv::Mat sample2 = cv::imread("Samples/Split_Sample2.png", cv::IMREAD_COLOR);

    std::cout << "Sample1 Type: " << sample1.type()
    << "| Pixel Size: " << sample1.elemSize() << std::endl;

    std::cout << sample1 << std::endl;
    std::cout << "Sample2 Type: " << sample2.type() << std::endl;
    std::cout << sample2 << std::endl;

    sample1.convertTo(sample1, CV_16S);
    sample2.convertTo(sample2, CV_16S);

    cv::Mat diff(sample1.size(), 19);
    cv::subtract(sample2, sample1, diff);

    std::cout << "Difference: " << diff.type() << " | Size: " << diff.elemSize() << std::endl;
    std::cout << diff << std::endl;

    cv::Mat diff8U(diff.size(), 16);

    for (size_t i = 0; i < diff.rows; i++) {
        for (size_t j = 0; j < diff.cols; j++) {
            for (short k = 0; k < 3; k++) {
                const auto pK = diff.at<cv::Vec3s>(i, j)[k];
                if (pK < 0) {
                    diff8U.at<cv::Vec3b>(i, j)[k] = pK + 256;
                } else {
                    diff8U.at<cv::Vec3b>(i, j)[k] = pK;
                }
            }
        }
    }

    std::cout << diff8U << std::endl;
    // Reconstruct the sample2 from diff8U and sample1

    sample1.convertTo(sample1, CV_8U);
    cv::Mat s2Constructed(diff8U.size(), 16);

    for (size_t i = 0; i < diff8U.rows; i++) {
        for (size_t j = 0; j < diff8U.cols; j++) {
            for (short k = 0; k < 3; k++) {
                const int dK = static_cast<int>(diff8U.at<cv::Vec3b>(i, j)[k]) + sample1.at<cv::Vec3b>(i, j)[k];
                if (dK < 255) {
                    s2Constructed.at<cv::Vec3b>(i, j)[k] = dK - 256;
                } else {
                    s2Constructed.at<cv::Vec3b>(i, j)[k] = dK;
                }
            }
        }
    }

    std::cout << s2Constructed << std::endl;


    return 0;
}

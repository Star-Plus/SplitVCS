#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {


    // Mat img = imread("image.jpg");
    // Mat imgEdited = imread("image_edited.jpg");
    //
    // if (img.empty() || imgEdited.empty()) {
    //     cout << "Could not open or find the images!" << endl;
    //     return -1;
    // }
    //
    // if (img.size() != imgEdited.size() || img.type() != imgEdited.type()) {
    //     cout << "Images must be of the same size and type!" << endl;
    //     return -1;
    // }
    //
    // Mat diff;
    // absdiff(img, imgEdited, diff);
    // imwrite("diff.jpg", diff);

    Mat img = imread("image.jpg");
    Mat diff = imread("diff.jpg");

    Mat img_16s, diff_16s;
    img.convertTo(img_16s, CV_16S);
    diff.convertTo(diff_16s, CV_16S);

    Mat reconstructed = img_16s + diff_16s;
    reconstructed.convertTo(reconstructed, CV_8U);
    imwrite("reconstructed.jpg", reconstructed);

    return 0;
}

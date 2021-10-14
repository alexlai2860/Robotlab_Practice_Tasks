//
//created by alex on 21-9-12
//edited by alex on 21-9-18
//

#include "armor_identify.h"
#include <iostream>

using namespace std;

/*计算高宽比*/
static double hw_rate(const cv::RotatedRect &Rect)
{
    if (Rect.size.height > Rect.size.width)
    {
        return Rect.size.height / Rect.size.width;
    }
    else
    {
        return Rect.size.width / Rect.size.height;
    }
}
/*计算面积比*/
static double areaRatio(const std::vector<cv::Point> &contour, const cv::RotatedRect &Rect)
{
    return cv::contourArea(contour) / Rect.size.area();
}
/*定义灯条识别函数*/
bool AutoAim::LightBlobsidentify(cv::Mat &src, LightBlob &lightblob)
{
    vector<cv::Mat> channels;
    vector<cv::Vec4i> hireachy;
    vector<vector<cv::Point>> contours;
    cv::Mat gray_image, binary_image;
    cv::Mat gray_image2 = gray_image.clone();
    cv::Mat binary_image2 = binary_image.clone();
    cv::Mat result = src.clone();
    /*初次二值化*/
    cv::cvtColor(src, gray_image2, CV_BGR2GRAY);
    cv::threshold(gray_image2, binary_image2, 225, 255, CV_THRESH_BINARY);
    cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    cv::dilate(binary_image2, binary_image2, element);
    //cv::imshow("binary_image2", binary_image2);

    /*分离色彩通道*/
    cv::split(src, channels);
    /*预处理删除己方装甲板颜色,再次二值化*/
    string R = "RED";
    string C = lightblob.blob_color;
    if (R == C)
        gray_image = channels.at(2) - channels.at(0); /*红色通道减去蓝色通道*/
    else
        gray_image = channels.at(0) - channels.at(2); /*蓝色通道减去红色通道*/
    cv::threshold(gray_image, binary_image, 125, 255, CV_THRESH_BINARY);
    cv::Mat element2 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    cv::dilate(binary_image, binary_image, element2);
    cv::imshow("binary_image1", binary_image);

    /*图像相加*/
    cv::Mat add_image = binary_image + binary_image2;
    //cv::GaussianBlur(add_image, add_image, cv::Size(7, 7), 2, 2);
    cv::Mat element3 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    cv::dilate(add_image, add_image, element3);
    //cv::imshow("add_image", add_image);

    /*寻找边缘*/
    cv::Canny(add_image, add_image, 100, 240, 3, false);
    //cv::imshow("add_image2", add_image);
    cv::findContours(add_image, contours, hireachy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE, cv::Point());
    // cout << contours.size() << endl;
    int i;
    lightblob.num = 0;
    double minarea[500];
    for (i = 1; i < contours.size(); i++)
    {
        //cv::drawContours(result, contours, i, cv::Scalar(128, 0, 128), 3, 8, cv::Mat(), 0, cv::Point());
        minarea[i] = cv::contourArea(contours[i]);
        if (minarea[i] > 200)
        {
            cv::RotatedRect rect[100];
            rect[i] = cv::minAreaRect(contours[i]);
            auto r1 = hw_rate(rect[i]);
            // cout << r1 << endl;
            if (r1 > 2.5)
            {
                auto r2 = areaRatio(contours[i], rect[i]);
                if (0.4 < r2)
                {
                    lightblob.num++;
                    cv::Rect rec = rect[i].boundingRect();
                    cv::rectangle(src, rec, cv::Scalar(255, 0, 225), 5, 1, 0);
                    lightblob.rect[lightblob.num - 1] = rect[i];
                }
            }
        }
    }
    //cv::imshow("contours", result);
    //cv::waitKey(100);
    // cv::imshow("light_blobs", src);
    // cout << lightblob.num << endl;
    return lightblob.num >= 2;
}
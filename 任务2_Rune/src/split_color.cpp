/**
 * @file split_color.cpp
 * @author alex
 * @brief 预处理图像
 * @version 0.2
 * @date 2021-09-30
 * 
 * alex all rights reserved
 * 
 */

#include "energy.h"
#include <iostream>

using namespace std;

//图像预处理函数
cv::Mat SplitColor::splitColor(cv::Mat &src, vector<cv::Mat> &channels, Energy &energy)
{
    cv::Mat gray_image, binary_image;
    //分离色彩通道，预处理删除己方装甲板颜色,二值化
    cv::split(src, channels);
    string R = "RED";
    string C = energy.getEnemyColor();
    //红色通道减去蓝色通道
    if (R == C)
        gray_image = channels.at(2) - channels.at(0);
    //蓝色通道减去红色通道
    else
        gray_image = channels.at(0) - channels.at(2);
    cv::threshold(gray_image, binary_image, this->threshold_min, this->threshold_max, CV_THRESH_BINARY);
    cv::GaussianBlur(binary_image, binary_image, cv::Size(this->guss_size, this->guss_size), 2, 2);
    cv::Mat element2 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(this->dilate_element_size, this->dilate_element_size));
    cv::dilate(binary_image, binary_image, element2);
    return binary_image;
}
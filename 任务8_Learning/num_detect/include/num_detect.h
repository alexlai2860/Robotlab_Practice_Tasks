/**
 * @file num_detect.h
 * @author alex
 * @brief 
 * @version 0.1
 * @date 2021-10-08
 * 
 * alex all rights reserved
 * 
 */
#pragma once

#include <algorithm>
#include <iostream>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace cv::dnn;

class Dnn_NumDetect
{
public:
    Dnn_NumDetect() = default;
    void loadModel(const string &path);
    void Classification();

private:
    Net lenet_5;
};

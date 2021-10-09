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

#include <iostream>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace cv::dnn;

class Dnn_NumDetect
{
public:
    void Classification();
    Dnn_NumDetect(const string &path);

private:
    // 加载onnx模型
    void loadModel(const string &path);
    //声明lenet_5模型
    Net lenet_5;
};

/**
 * @file main.cpp
 * @author alex
 * @brief 
 * @version 0.1
 * @date 2021-10-08
 * 
 * alex all rights reserved
 * 
 */

#include "num_detect.h"

int main()
{
    string path = "MNIST.onnx";
    Dnn_NumDetect detect1(path);

    Mat src = imread("test.png");
    imshow("src", src);
    waitKey(1000);
    detect1.Classification();
    return 0;
}

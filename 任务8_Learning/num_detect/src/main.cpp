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
    Dnn_NumDetect detect1;
    detect1.loadModel(path);
    detect1.Classification();
    return 0;
}

#include "num_detect.h"

void Dnn_NumDetect::loadModel(const string &path)
{
    string model = path;
    this->lenet_5 = readNetFromONNX(model);
    this->lenet_5.setPreferableBackend(DNN_BACKEND_OPENCV);
    this->lenet_5.setPreferableTarget(DNN_TARGET_CPU);
}

void Dnn_NumDetect::Classification()
{
    Mat src = imread("testnum2-1.png");
    imshow("src", src);
    waitKey(1000);
    //输入图片预处理
    cv::resize(src, src, Size(28, 28));
    cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);
    Mat inputBolb = blobFromImage(src, 0.00392157f, Size(28, 28), Scalar(), false, false);
    this->lenet_5.setInput(inputBolb);
    //向前传播
    Mat prob = this->lenet_5.forward();
    cout << prob << endl;

    //输出可能性最大的数字
    float num[10];
    for (int i = 0; i < 10; i++)
    {
        num[i] = prob.at<float>(i);
    }
    int maxnum = max_element(num, num + 6) - num;
    cout << maxnum << endl;
}
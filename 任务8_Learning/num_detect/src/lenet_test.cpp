#include "num_detect.h"

Dnn_NumDetect::Dnn_NumDetect(const string &path)
{
    this->loadModel(path);
}

void Dnn_NumDetect::loadModel(const string &path)
{
    string model = path;
    this->lenet_5 = readNetFromONNX(model);
    this->lenet_5.setPreferableBackend(DNN_BACKEND_OPENCV);
    this->lenet_5.setPreferableTarget(DNN_TARGET_CPU);
    /*     int input_height = 28, input_width = 28;
    double scale = 1.0 / 255;
    cv::Scalar mean(0, 0, 0);
    this->lenet_5.setInputParams(scale, cv::Size(input_width, input_height), mean); */
}

void Dnn_NumDetect::Classification()
{
    Mat src = imread("test.jpeg");
    imshow("src", src);
    waitKey(10);
    cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);
    Mat inputBolb = blobFromImage(src, 0.00392157f, Size(28, 28), Scalar(), false, false);
    // Mat input;
    // input = blobFromImage(src);
    // this->lenet_5.setInputsNames;
    this->lenet_5.setInput(inputBolb);
    Mat prob = this->lenet_5.forward();
    cout << prob << endl;

    Point classIdPoint;
    double confidence;
    //查找最大值和最小值
    minMaxLoc(prob.reshape(1, 1), 0, &confidence, 0, &classIdPoint);
    int classId = classIdPoint.x;
    cout << classId << endl;
}
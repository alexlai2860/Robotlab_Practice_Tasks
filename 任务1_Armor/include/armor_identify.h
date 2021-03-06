//
//created by alex on 21-9-11
//edited by alex on 21-9-18
//
#pragma once

#include <opencv2/imgproc/types_c.h>
#include <opencv2/opencv.hpp>

using namespace std;

class ArmorBox
{
public:
    cv::Point2f center;
    cv::RotatedRect rect[30];
    int box_num;
    ArmorBox(string &c) : box_color(c){};
    ~ArmorBox() {}

private:
    string box_color;
};

class LightBlob
{
public:
    cv::RotatedRect rect[100];
    double length;
    string blob_color;
    int num;
    LightBlob(string &c) : blob_color(c){};
    ~LightBlob() {}
};

class PNP
{
public:
    void setVertex(cv::Point2f p[4]);
    cv::Point2f getVertex(int i);
    void getPosition();
    void getEulerAngle();
    PNP() = default;
    ~PNP() {}

private:
    cv::Point2f vertex[4];
};

class AutoAim
{
public:
    AutoAim() = default;
    ~AutoAim() {}
    bool lightBlobsIdentify(cv::Mat &src, LightBlob &lightblob);
    bool armorBoxIdentify(cv::Mat &src, LightBlob &lightblob, ArmorBox &armorbox, PNP &pnp);
};

class Performance
{
public:
    double t, fps;
    int n, framenum;
    void get_fps(double &t, double &fps);
    void identified_rate(int &n, int &framenum);
    Performance()
    {
        t = 0;
        fps = 0;
        n = 0;
        framenum = 0;
    }
    ~Performance() {}
};
class Kalman
{
public:
    void kalmanFlitertest(cv::Mat &src, ArmorBox &points, cv::KalmanFilter &KF, cv::Mat &measurement);
    Kalman() = default;
};
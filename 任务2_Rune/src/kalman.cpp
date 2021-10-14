/**
 * @file kalman.cpp
 * @author alex
 * @brief 
 * @version 0.1
 * @date 2021-10-14
 * 
 * alex all rights reserved
 * 
 */

#include "energy.h"
#include <iostream>
using namespace std;
using namespace cv;

static inline Point calcPoint(Point2f center, double R, double angle)
{
    return center + Point2f((float)cos(angle), (float)-sin(angle)) * (float)R;
}

double get_distance(cv::Point2f &p1, cv::Point2f &p2)
{
    double distance = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
    return distance;
}

Point2f EnergyIdentify::getPoint(int i)
{
    Point2f center;
    if (i == 1)
    {
        center = this->armor_center;
    }
    else if (i == 2)
    {
        center = this->circle_center;
    }
    return center;
}

void Kalman::kalmanFlitertest(Mat &src, EnergyIdentify &points, KalmanFilter &KF, Mat &measurement)
{
    //2.预测
    Mat prediction = KF.predict();
    Point predict_pt = Point(prediction.at<float>(0), prediction.at<float>(1)); //预测值(x',y')

    //3.更新
    measurement.at<float>(0) = points.getPoint(1).x;
    measurement.at<float>(1) = points.getPoint(1).y;

    KF.correct(measurement);

    circle(src, predict_pt, 3, Scalar(0, 255, 0), 3);

}

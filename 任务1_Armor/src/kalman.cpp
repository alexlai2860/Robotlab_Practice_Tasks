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

#include "armor_identify.h"
#include <iostream>
using namespace std;
using namespace cv;

static inline Point calcPoint(Point2f center, double R, double angle)
{
    return center + Point2f((float)cos(angle), (float)-sin(angle)) * (float)R;
}

void Kalman::kalmanFlitertest(Mat &src, ArmorBox &points, KalmanFilter &KF, Mat &measurement)
{
    //2.预测
    Mat prediction = KF.predict();
    Point predict_pt = Point(prediction.at<float>(0), prediction.at<float>(1)); //预测值(x',y')

    //3.更新
    measurement.at<float>(0) = points.center.x;
    measurement.at<float>(1) = points.center.y;

    KF.correct(measurement);

    circle(src, predict_pt, 3, Scalar(0, 255, 0), 3);

}

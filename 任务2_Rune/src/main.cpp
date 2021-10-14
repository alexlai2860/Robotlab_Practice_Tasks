/**
 * @file main.cpp
 * @author alex
 * @brief 主函数
 * @version 1.1
 * @date 2021-09-25
 * 
 * alex all rights reserved
 * 
 */

#include "energy.h"
#include "energy_param.h"
#include <iostream>

using namespace std;

int main()
{
    cv::Mat src, frame;
    cv::VideoCapture capture("energy.avi");
    string color = "BLUE";
    Energy energy1(color);
    EnergyIdentify identify1;
    Compare compare1;
    rCompare rcompare1;
    Energy_param e;
    SplitColor split_color;
    Kalman kalman;
    identify1.Init(e);
    split_color.splitSettingInit();

    //1.初始化
    const int stateNum = 4;   //状态值
    const int measureNum = 2; //测量值
    cv::KalmanFilter KF(stateNum, measureNum, 0);

    KF.transitionMatrix = (cv::Mat_<float>(4, 4) << 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1); //转移矩阵A
    cv::setIdentity(KF.measurementMatrix);                                                            //测量矩阵H
    cv::setIdentity(KF.processNoiseCov, cv::Scalar::all(1e-5));                                       //系统噪声方差矩阵Q
    cv::setIdentity(KF.measurementNoiseCov, cv::Scalar::all(1e-1));                                   //测量噪声方差矩阵R
    cv::setIdentity(KF.errorCovPost, cv::Scalar::all(1));                                             //后验错误估计协方差矩阵P
    cv::randn(KF.statePost, cv::Scalar::all(0), cv::Scalar::all(0.1));                                //初始状态值x(0)
    cv::Mat measurement = cv::Mat::zeros(measureNum, 1, CV_32F);                                      //初始测量值x'(0)

    while (1)
    {
        capture >> frame;
        if (frame.data != 0)
        {
            src = frame;
            cv::imshow("src", src);
            cv::waitKey(10);
            if (identify1.fansIdentify(src, energy1, e, compare1, split_color))
            {
                identify1.circleCenterIdentify(src, energy1, e, rcompare1, split_color);
                kalman.kalmanFlitertest(src, identify1, KF, measurement);
            }
            cv::imshow("fan", src);
            cv::waitKey(20);
        }
        else
        {
            cout << "no input" << endl;
            break;
        }
    }
    return 0;
}
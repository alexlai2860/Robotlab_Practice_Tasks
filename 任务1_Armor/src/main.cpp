//
//created by alex on 21-9-12
//edited by alex on 21-9-18
//

#include "armor_identify.h"
#include <iostream>

using namespace std;

int main()
{
    cv::Mat src, frame;
    cv::VideoCapture capture("006.avi"); /*可更改视频路径,PNP测试为006.avi,小陀螺为007.avi*/
    Performance p1;
    p1.t = cv::getTickCount();
    //1.初始化
    const int stateNum = 4;   //状态值
    const int measureNum = 2; //测量值
    cv::KalmanFilter KF(stateNum, measureNum, 0);

    KF.transitionMatrix = (cv::Mat_<float>(4, 4) << 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1); //转移矩阵A
    cv::setIdentity(KF.measurementMatrix);                                                           //测量矩阵H
    cv::setIdentity(KF.processNoiseCov, cv::Scalar::all(1e-5));                                      //系统噪声方差矩阵Q
    cv::setIdentity(KF.measurementNoiseCov, cv::Scalar::all(1e-1));                                  //测量噪声方差矩阵R
    cv::setIdentity(KF.errorCovPost, cv::Scalar::all(1));                                            //后验错误估计协方差矩阵P
    cv::randn(KF.statePost, cv::Scalar::all(0), cv::Scalar::all(0.1));                               //初始状态值x(0)
    cv::Mat measurement = cv::Mat::zeros(measureNum, 1, CV_32F);                                     //初始测量值x'(0)
    while (1)
    {
        capture >> frame;
        if (frame.data != 0)
        {
            p1.framenum++;
            src = frame;
            //cout << src.cols << " " << src.rows << endl;
            // cv::imshow("src image", src);
            string c = "RED"; /*若装甲板为红色，则改成""RED"*/
            LightBlob lb1(c);
            ArmorBox ab1(c);
            AutoAim a1;
            PNP pnp1;
            Kalman kalman1;
            auto src2 = src.clone();
            if (a1.LightBlobsidentify(src, lb1))
            {
                if (a1.ArmorBoxidentify(src2, lb1, ab1, pnp1))
                {
                    p1.n++;
                    pnp1.get_position();
                    kalman1.kalmanFlitertest(src2, ab1, KF, measurement);
                }
            }
            cv::imshow("armor box1", src2);
            cv::waitKey(1);
        }
        else
        {
            cout << "no input" << endl;
            break;
        }
    }
    /*输出识别率和平均帧率*/
    p1.identified_rate(p1.n, p1.framenum);
    p1.get_fps(p1.t, p1.fps);
    return 0;
}
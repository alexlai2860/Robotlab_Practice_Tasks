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
    identify1.Init(e);
    split_color.splitSettingInit();
    while (1)
    {
        capture >> frame;
        if (frame.data != 0)
        {
            src = frame;
            cv::imshow("src", src);
            cv::waitKey(10);
            if (identify1.fansIdentify(src, energy1, e, compare1,split_color))
            {
                identify1.circleCenterIdentify(src, energy1, e, rcompare1,split_color);
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
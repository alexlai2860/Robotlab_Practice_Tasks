/**
 * @file center_identify.cpp
 * @author alex
 * @brief 识别能量机关旋转中心(R)
 * @version 0.1
 * @date 2021-09-27
 * 
 * alex all rights reserved
 * 
 */

#include "energy.h"
#include "energy_param.h"
#include <iostream>

using namespace std;

//限制中心R的面积大小
bool rCompare::areaJudge(float &area, float &max_area, float &min_area)
{
    return (area > min_area && area < max_area);
}

//中心识别函数
void EnergyIdentify::circleCenterIdentify(cv::Mat &src, Energy &energy, Energy_param &energy_param, rCompare &rcompare, SplitColor &split_color)
{
    vector<cv::Mat> channels;
    vector<cv::Vec4i> hireachy;
    vector<vector<cv::Point>> contours;
    cv::Mat result = src.clone();
    int count = 0;
    //图像预处理，同扇叶识别
    cv::Mat binary_image = split_color.splitColor(src, channels, energy);
    //寻找边缘
    cv::findContours(binary_image, contours, hireachy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE, cv::Point());
    for (int i = 0; i < contours.size(); i++)
    {
        float area[100];
        area[i] = cv::contourArea(contours[i]);
        if (rcompare.areaJudge(area[i], energy_param.r_area_max, energy_param.r_area_min))
        {
            cv::RotatedRect rect[100];
            rect[i] = cv::minAreaRect(contours[i]);
            //判断长宽比
            if (rcompare.hwRate(rect[i], energy_param.r_hwrate_max, energy_param.r_hwrate_min))
            {
                cv::Rect rec = rect[i].boundingRect();
                cv::rectangle(src, rec, cv::Scalar(0, 215, 255), 1, 8, 0);
            }
        }
    }
}

/**
 * @file fans_identify.cpp
 * @author alex
 * @brief 识别待打击的扇叶和装甲板，并标注打击点
 * @version 0.4
 * @date 2021-09-25
 * 
 * alex all rights reserved
 * 
 */

#include "energy.h"
#include "energy_param.h"
#include <iostream>

using namespace std;

//添加扇叶
void Energy::addFan(cv::RotatedRect &rect)
{
    fan.push_back(rect);
}

//添加装甲板
void Energy::addArmor(cv::RotatedRect &rect)
{
    armor = rect;
}

//获取装甲板
cv::RotatedRect Energy::getArmor()
{
    return this->armor;
}

//获取敌方颜色
string Energy::getEnemyColor()
{
    return this->energy_color;
}

//长宽比限制函数
bool Compare::hwRate(cv::RotatedRect &rect, float &up_rate, float &low_rate)
{
    float hw_rate;
    if (rect.size.height > rect.size.width)
    {
        hw_rate = rect.size.height / rect.size.width;
    }
    else
    {
        hw_rate = rect.size.width / rect.size.height;
    }
    return (hw_rate > low_rate && hw_rate < up_rate);
}

//旋转矩形与轮廓面积比限制函数
bool Compare::areaRate(cv::RotatedRect &rect, float &contour_area, float &up_rate, float &low_rate)
{
    float rect_area = rect.size.height * rect.size.width;
    float area_rate = contour_area / rect_area;
    return (area_rate > low_rate && area_rate < up_rate);
}

//扇叶＆装甲板＆打击点识别函数
bool EnergyIdentify::fansIdentify(cv::Mat &src, Energy &energy, Energy_param &energy_param, Compare &compare, SplitColor &split_color)
{
    vector<cv::Mat> channels;
    vector<cv::Vec4i> hireachy;
    vector<vector<cv::Point>> contours;
    cv::Mat result = src.clone();
    int count = 0;
    //预处理图像
    cv::Mat binary_image = split_color.splitColor(src, channels, energy);
    //提取轮廓
    cv::findContours(binary_image, contours, hireachy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE, cv::Point());
    for (int i = 0; i < contours.size(); i++)
    {
        float area[100];
        area[i] = cv::contourArea(contours[i]);
        if (area[i] > energy_param.contour_area_min)
        {
            cv::RotatedRect rect[100];
            rect[i] = cv::minAreaRect(contours[i]);
            //判断长宽比
            if (compare.hwRate(rect[i], energy_param.fan_hwrate_max, energy_param.fan_hwrate_min))
            {
                float area_rate_max = energy_param.fan_area_max / area[i]; //计算最大面积比
                float area_rate_min = energy_param.fan_area_min / area[i]; //计算最小面积比
                //判断旋转矩形与轮廓面积比
                if (compare.areaRate(rect[i], area[i], area_rate_max, area_rate_min))
                {
                    energy.addFan(rect[i]);
                    cv::Rect rec = rect[i].boundingRect();
                    // 当所提取出的轮廓存在子轮廓时，绘制旋转矩形框
                    if (hireachy[i][2] != -1)
                    {
                        int num = hireachy[i][2];
                        double length = cv::arcLength(contours[num], true);
                        //限制装甲板周长，防止误识别流动条
                        if (length < energy_param.armor_contour_length_max && length > energy_param.armor_contour_length_min)
                        {
                            count = 1;
                            cv::RotatedRect armor = energy.getArmor();
                            armor = cv::minAreaRect(contours[num]);
                            cv::Point2f vertex[4];
                            armor.points(vertex);
                            for (int i = 0; i < 4; i++)
                            {
                                //绘制装甲板矩形框和打击点
                                cv::line(src, vertex[i], vertex[(i + 1) % 4], cv::Scalar(0, 0, 255), 1, 8, 0);
                                cv::Point2f armor_center((vertex[1].x + vertex[3].x) / 2, (vertex[1].y + vertex[3].y) / 2);
                                cv::circle(src, armor_center, 2, cv::Scalar(255, 255, 255), 2, 8, 0);
                            }
                        }
                    }
                }
            }
        }
    }
    return count;
}
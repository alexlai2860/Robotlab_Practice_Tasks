/**
 * @file energy.h
 * @author alex
 * @brief 头文件，包括各种类
 * @version 0.3
 * @date 2021-09-25
 * 
 * alex all rights reserved
 * 
 */

#pragma once

#include "energy_param.h"
#include <opencv2/imgproc/types_c.h>
#include <opencv2/opencv.hpp>

using namespace std;

//能量机关类
class Energy
{
public:
    int count;
    void addFan(cv::RotatedRect &rect);
    void addArmor(cv::RotatedRect &rect);
    void addFanContour(vector<vector<cv::Point2f>> &contour);
    cv::RotatedRect getArmor();
    string getEnemyColor();
    Energy(string &c) : energy_color(c){};
    ~Energy() {}

private:
    string energy_color;
    cv::RotatedRect armor;
    vector<vector<cv::Point2f>> fan_contour;
    vector<cv::RotatedRect> fan;
};

//参数比较类（用于寻找待击打的扇叶）
class Compare
{
public:
    bool hwRate(cv::RotatedRect &rect, float &up_rate, float &low_rate);                        //限制长宽比
    bool areaRate(cv::RotatedRect &rect, float &contour_area, float &up_rate, float &low_rate); //限制面积比
    Compare() = default;
    ~Compare() {}
};

//圆心‘Ｒ’参数比较类（Compare的子类）
class rCompare : public Compare
{
public:
    bool areaJudge(float &area, float &max_area, float &min_area);
    rCompare() = default;
    ~rCompare() {}
};

// 通道分离及预处理类
class SplitColor
{
public:
    void splitSettingInit();
    cv::Mat splitColor(cv::Mat &src, vector<cv::Mat> &channels, Energy &energy);
    SplitColor() = default;
    ~SplitColor() {}

private:
    int threshold_min;
    int threshold_max;
    int guss_size;
    int dilate_element_size;
};

//能量机关识别类（包括初始化函数和识别函数）
class EnergyIdentify
{
public:
    void Init(Energy_param &e);
    void circleCenterIdentify(cv::Mat &src, Energy &energy, Energy_param &energy_param, rCompare &rcompare, SplitColor &);
    bool fansIdentify(cv::Mat &src, Energy &energy, Energy_param &energy_param, Compare &compare, SplitColor &); //找到待击打扇叶和装甲板
    cv::Point2f getPoint(int i);
    EnergyIdentify() = default;
    ~EnergyIdentify() {}

private:
    void energyInit(Energy_param &e);
    void circleCenterInit(Energy_param &e);
    cv::Point2f armor_center;
    cv::Point2f circle_center;
};

class Kalman
{
public:
    void kalmanFlitertest(cv::Mat &src, EnergyIdentify &points, cv::KalmanFilter &KF, cv::Mat &measurement);
    Kalman() = default;
};
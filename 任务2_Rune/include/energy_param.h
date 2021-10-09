/**
 * @file energy_param.h
 * @author alex
 * @brief 定义各参数
 * @version 0.1
 * @date 2021-09-25
 * 
 * alex all rights reserved
 * 
 */

#pragma once

using namespace std;

struct Energy_param
{
    float fan_hwrate_max;            //扇叶长宽比最大值
    float fan_hwrate_min;            //扇叶长宽比最小值
    float fan_area_max;              //扇叶面积最大值
    float fan_area_min;              //扇叶面积最小值
    float contour_area_max;          //轮廓面积最大值
    float contour_area_min;          //轮廓面积最小值
    double armor_contour_length_max; //装甲板轮廓周长最大值
    double armor_contour_length_min; //装甲板轮廓周长最小值
    float r_area_max;                //Ｒ字样面积最大值
    float r_area_min;                //Ｒ字样面积最小值
    float r_hwrate_max;              //Ｒ字样长宽比最大值
    float r_hwrate_min;              //Ｒ字样长宽比最小值
};

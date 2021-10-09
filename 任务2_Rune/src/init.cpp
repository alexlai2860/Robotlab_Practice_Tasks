/**
 * @file energy_init.cpp
 * @author alex
 * @brief 初始化各项参数
 * @version 0.2
 * @date 2021-09-25
 * 
 * alex all rights reserved
 * 
 */

#include "energy.h"
#include "energy_param.h"

using namespace std;

void EnergyIdentify::Init(Energy_param &e)
{
    this->circleCenterInit(e);
    this->energyInit(e);
}

void EnergyIdentify::energyInit(Energy_param &e)
{
    e.contour_area_max = 1500;
    e.contour_area_min = 1200;
    e.fan_area_max = 2500;
    e.fan_area_min = 1800;
    e.fan_hwrate_max = 3;
    e.fan_hwrate_min = 1.5;
    e.armor_contour_length_max = 100;
    e.armor_contour_length_min = 70;
}

void EnergyIdentify::circleCenterInit(Energy_param &e)
{
    e.r_area_max = 500;
    e.r_area_min = 300;
    e.r_hwrate_max = 1.4;
    e.r_hwrate_min = 0.7;
}

void SplitColor::splitSettingInit()
{
    this->threshold_max = 255;
    this->threshold_min = 70;
    this->dilate_element_size = 7;
    this->guss_size = 3;
}

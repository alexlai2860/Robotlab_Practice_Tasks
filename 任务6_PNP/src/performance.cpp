//
//created by alex on 21-9-18
//

#include"armor_identify.h"
#include<iostream>

using namespace std;

void Performance::identified_rate(int &n,int &framenum)
{
    double rate = (double)n / framenum;
    cout <<"the identify rate : "<<100 * rate <<" %"<< endl;
}
void Performance::get_fps(double &t, double &fps)
{
    t = cv::getTickCount() - t;
    fps = framenum / (t / cv::getTickFrequency());
    cout <<"the fps : "<<fps << endl;
}


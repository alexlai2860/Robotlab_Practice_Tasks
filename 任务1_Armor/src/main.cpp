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
    cv::VideoCapture capture("006.avi"); /*可更改视频路径*/
    Performance p1;
    p1.t = cv::getTickCount();
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
            auto src2 = src.clone();
            if (a1.LightBlobsidentify(src, lb1))
            {
                if (a1.ArmorBoxidentify(src2, lb1, ab1, pnp1))
                {
                    p1.n++;
                    pnp1.get_position();
                }
            }
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
//
//created by alex on 21-9-12
//edited by alex on 21-9-17
//

#include "armor_identify.h"
#include <algorithm>

/*获取灯条长度*/
/*别问为什么会有这个函数，问就是考虑不周的补救措施*/
double lightblob_length(const cv::RotatedRect &light_blob)
{
    if (light_blob.size.width > light_blob.size.height)
    {
        return light_blob.size.width;
    }
    else
        return light_blob.size.height;
}

/* 判断两个灯条的角度差*/
static bool angelJudge(const cv::RotatedRect &light_blob_i, const cv::RotatedRect &light_blob_j)
{
    float angle_i = light_blob_i.size.width > light_blob_i.size.height ? light_blob_i.angle : light_blob_i.angle + 90;
    float angle_j = light_blob_j.size.width > light_blob_j.size.height ? light_blob_j.angle : light_blob_j.angle + 90;
    return abs(angle_i - angle_j) < 4.5;
}
/* 判断两个灯条的高度差*/
static bool heightJudge(const cv::RotatedRect &light_blob_i, const cv::RotatedRect &light_blob_j)
{
    float height = light_blob_i.center.y - light_blob_j.center.y;
    return abs(height) < 100;
}
/*判断两个灯条的间距*/
static bool lengthJudge(const cv::RotatedRect &light_blob_i, const cv::RotatedRect &light_blob_j)
{
    double length_i = lightblob_length(light_blob_i);
    double length_j = lightblob_length(light_blob_j);
    double side_length;
    side_length = sqrt(pow(light_blob_i.center.x - light_blob_j.center.x, 2) + pow(light_blob_i.center.y - light_blob_j.center.y, 2));
    return (side_length / length_i < 2.5 && side_length / length_i > 0.75);
}
/* 判断两个灯条的长度比*/
static bool lengthRatioJudge(const cv::RotatedRect &light_blob_i, const cv::RotatedRect &light_blob_j)
{
    double length_i = lightblob_length(light_blob_i);
    double length_j = lightblob_length(light_blob_j);
    return (length_i / length_j < 1.5 && length_i / length_j > 0.7);
}
/* 判断是否匹配*/
static bool isCoupleLight(const cv::RotatedRect &light_blob_i, const cv::RotatedRect &light_blob_j)
{
    return lengthRatioJudge(light_blob_i, light_blob_j) &&
           lengthJudge(light_blob_i, light_blob_j) &&
           heightJudge(light_blob_i, light_blob_j) &&
           angelJudge(light_blob_i, light_blob_j);
}
cv::Point2f findcenter(const cv::RotatedRect &light_blob_i, const cv::RotatedRect &light_blob_j, ArmorBox &armorbox)
{
    armorbox.center = cv::Point2f(light_blob_j.center.x / 2 + light_blob_i.center.x / 2, light_blob_i.center.y / 2 + light_blob_j.center.y / 2);
    return armorbox.center;
}
/*返回长边和对角线中的最短长度，用于排除菱形*/
double diagonaljudge(cv::Point2f &p1, cv::Point2f &p2, cv::Point2f &p3, cv::Point2f &p4)
{
    double l1 = sqrt((pow(p1.x - p3.x, 2) + pow(p1.y - p3.y, 2)));
    double l2 = sqrt((pow(p2.x - p4.x, 2) + pow(p2.y - p4.y, 2)));
    double l3 = sqrt((pow(p1.x - p4.x, 2) + pow(p1.y - p4.y, 2)));
    double l4 = sqrt((pow(p2.x - p3.x, 2) + pow(p2.y - p3.y, 2)));
    double minlength = min({l1, l2, l3, l4});
    return minlength;
}

/*寻找装甲板并绘制*/
bool AutoAim::ArmorBoxidentify(cv::Mat &src, LightBlob &lightblob, ArmorBox &armorbox, PNP &pnp)
{
    armorbox.box_num = 0;
    for (int i = 0; i < lightblob.num; i++)
    {
        for (int j = i + 1; j < lightblob.num; j++)
        {
            if (isCoupleLight(lightblob.rect[i], lightblob.rect[j]))
            {
                cv::Point2f p_i[4], p_j[4], p;
                lightblob.rect[i].points(p_i);
                lightblob.rect[j].points(p_j);
                cv::Point2f reci1, reci2, recj1, recj2;
                lightblob.rect[i].size.width > lightblob.rect[i].size.height ? reci1 = cv::Point2f(p_i[0].x / 2 + p_i[1].x / 2, p_i[0].y / 2 + p_i[1].y / 2) : reci1 = cv::Point2f(p_i[0].x / 2 + p_i[3].x / 2, p_i[0].y / 2 + p_i[3].y / 2);
                lightblob.rect[i].size.width > lightblob.rect[i].size.height ? reci2 = cv::Point2f(p_i[2].x / 2 + p_i[3].x / 2, p_i[2].y / 2 + p_i[3].y / 2) : reci2 = cv::Point2f(p_i[2].x / 2 + p_i[1].x / 2, p_i[2].y / 2 + p_i[1].y / 2);
                lightblob.rect[j].size.width > lightblob.rect[j].size.height ? recj1 = cv::Point2f(p_j[0].x / 2 + p_j[1].x / 2, p_j[0].y / 2 + p_j[1].y / 2) : recj1 = cv::Point2f(p_j[0].x / 2 + p_j[3].x / 2, p_j[0].y / 2 + p_j[3].y / 2);
                lightblob.rect[j].size.width > lightblob.rect[j].size.height ? recj2 = cv::Point2f(p_j[2].x / 2 + p_j[3].x / 2, p_j[2].y / 2 + p_j[3].y / 2) : recj2 = cv::Point2f(p_j[2].x / 2 + p_j[1].x / 2, p_j[2].y / 2 + p_j[1].y / 2);
                if (diagonaljudge(reci1, reci2, recj1, recj2) > 100)
                {
                    cv::Point2f center = findcenter(lightblob.rect[i], lightblob.rect[j], armorbox);
                    cv::line(src, reci1, center, cv::Scalar(0, 235, 255), 2);
                    cv::line(src, recj1, center, cv::Scalar(0, 235, 255), 2);
                    cv::line(src, reci2, center, cv::Scalar(0, 235, 255), 2);
                    cv::line(src, recj2, center, cv::Scalar(0, 235, 255), 2);
                    cv::circle(src, center, 2, cv::Scalar(0, 0, 255), 3);
                    // cv::Rect rec_i = lightblob.rect[i].boundingRect();
                    // cv::Rect rec_j = lightblob.rect[j].boundingRect();
                    // cv::rectangle(src, rec_j.tl(),rec_i.br(), cv::Scalar(255, 0, 225), 5, 1, 0);
                    armorbox.box_num++;
                    cv::Point2f p[4];
                    p[0] = reci1;
                    p[1] = reci2;
                    p[2] = recj1;
                    p[3] = recj2;
                    pnp.set_vertex(p);
                    cv::circle(src, pnp.get_vertex(0), 2, cv::Scalar(0, 0, 255), 2);
                    cv::circle(src, pnp.get_vertex(1), 4, cv::Scalar(0, 0, 255), 2);
                    cv::circle(src, pnp.get_vertex(2), 6, cv::Scalar(0, 0, 255), 2);
                    cv::circle(src, pnp.get_vertex(3), 8, cv::Scalar(0, 0, 255), 2);
                }
            }
        }
    }
    // cout << armorbox.box_num << endl;
    cv::imshow("armor box", src);
    cv::waitKey(1);
    return armorbox.box_num >= 1;
}
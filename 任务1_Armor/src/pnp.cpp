//
//created by alex on 21-10-04
//edited by alex on 21-10-04
//

#include "armor_identify.h"
#include <cmath>
#include <eigen3/Eigen/Dense>
#include <iomanip>
#include <opencv2/core/eigen.hpp>

using namespace std;
using namespace cv;

double get_distance(cv::Point2f &p1, cv::Point2f &p2)
{
    double distance = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
    return distance;
}
//对顶点进行重新排序，离坐标原点最近的为起点[0]
void PNP::setVertex(cv::Point2f p[4])
{
    double distance[4];
    for (int i = 0; i < 4; i++)
    {
        cv::Point2f center(0, 0);
        distance[i] = get_distance(p[i], center);
    }
    int min_distance_num = 0;
    for (int i = 0; i < 4; i++)
    {
        if (distance[i] < distance[min_distance_num])
        {
            min_distance_num = i;
        }
    }
    this->vertex[0] = p[min_distance_num];
    int each_distance[4];
    int min_distance[4];
    for (int i = 0; i < 4; i++)
    {
        each_distance[i] = get_distance(p[min_distance_num], p[i]);
    }
    for (int i = 0; i < 4; i++)
    {
        min_distance[i] = each_distance[i];
    }
    sort(min_distance, min_distance + 4);
    for (int i = 0; i < 4; i++)
    {
        if (min_distance[1] == each_distance[i])
        {
            this->vertex[3] = p[i];
        }
        else if (min_distance[2] == each_distance[i])
        {
            this->vertex[1] = p[i];
        }
        else if (min_distance[3] == each_distance[i])
        {
            this->vertex[2] = p[i];
        }
    }
}

cv::Point2f PNP::getVertex(int i)
{
    return this->vertex[i];
}

//用decomposeProjectionMatrix求解欧拉角
void getEulerAngles(cv::Mat pose_mat, cv::Vec3d euler_angle)
{

    cv::Mat out_intrinsics = cv::Mat(3, 3, CV_64FC1);
    cv::Mat out_rotation = cv::Mat(3, 3, CV_64FC1);
    cv::Mat out_translation = cv::Mat(4, 1, CV_64FC1);

    cv::decomposeProjectionMatrix(pose_mat, out_intrinsics, out_rotation, out_translation, cv::noArray(), cv::noArray(), cv::noArray(), euler_angle);

    double yaw = euler_angle[1];
    double pitch = euler_angle[0];
    double roll = euler_angle[2];

    cout << setprecision(4) << setiosflags(ios::left) << "yaw: " << setw(8) << yaw << "pitch: " << setw(8) << pitch << "roll: " << setw(8) << roll << endl;
}

// 检查是否是旋转矩阵
bool isRotationMatrix(cv::Mat &R)
{
    cv::Mat Rt;
    cv::transpose(R, Rt);
    cv::Mat shouldBeIdentity = Rt * R;
    cv::Mat I = cv::Mat::eye(3, 3, shouldBeIdentity.type());

    return norm(I, shouldBeIdentity) < 1e-6;
}

//直接算欧拉角
cv::Vec3f rotationMatrixToEulerAngles(cv::Mat &R)
{
    assert(isRotationMatrix(R));

    float sy = sqrt(R.at<double>(0, 0) * R.at<double>(0, 0) + R.at<double>(1, 0) * R.at<double>(1, 0));

    bool singular = sy < 1e-6; // 奇异性判断

    float x, y, z;
    if (!singular)
    {
        x = atan2(R.at<double>(2, 1), R.at<double>(2, 2));
        y = atan2(-R.at<double>(2, 0), sy);
        z = atan2(R.at<double>(1, 0), R.at<double>(0, 0));
    }
    else
    {
        x = atan2(-R.at<double>(1, 2), R.at<double>(1, 1));
        y = atan2(-R.at<double>(2, 0), sy);
        z = 0;
    }
    float yaw, pitch, roll;
    yaw = y * 180 / 3.1416;
    pitch = x * 180 / 3.1416;
    roll = z * 180 / 3.1416;
    cout << setprecision(4) << setiosflags(ios::left) << "yaw: " << setw(8) << yaw << "pitch: " << setw(8) << pitch << "roll: " << setw(8) << roll << endl;
    return cv::Vec3f(x, y, z);
}

//PNP解算主函数，获取相机位置(距离)及装甲板的姿态
void PNP::getPosition()
{
    cv::Mat camera_matrix;
    cv::Mat distortion_coefficients;
    cv::FileStorage fs("./camera_and_distortion_matrix.yml", cv::FileStorage::READ);
    fs["camera_matrix"] >> camera_matrix;
    fs["distortion_coefficients"] >> distortion_coefficients;

    vector<cv::Point3f> armor_vertex1;
    armor_vertex1.push_back(cv::Point3f(-33.75f, -13.25f, 0));
    armor_vertex1.push_back(cv::Point3f(+33.75f, -13.25f, 0));
    armor_vertex1.push_back(cv::Point3f(+33.75f, +13.25f, 0));
    armor_vertex1.push_back(cv::Point3f(-33.75f, +13.25f, 0));

    vector<cv::Point2f> armor_vertex2;
    for (int i = 0; i < 4; i++)
    {
        armor_vertex2.push_back(this->getVertex(i));
    }

    cv::Mat Rvec;
    cv::Mat_<float> Tvec;
    cv::Mat raux, taux;

    cv::solvePnP(armor_vertex1, armor_vertex2, camera_matrix, distortion_coefficients, raux, taux);

    raux.convertTo(Rvec, CV_64FC1); //旋转向量
    taux.convertTo(Tvec, CV_64FC1); //平移向量

    cv::Mat rotMat;
    cv::Rodrigues(raux, rotMat); //由于solvePnP返回的是旋转向量，故用罗德里格斯变换变成旋转矩阵

    /* //格式转换,并用engle库求解三个轴偏转的角度
    Eigen::Matrix3f R_n;
    Eigen::Matrix3f T_n;
    cv2eigen(rotMat, R_n);
    cv2eigen(Tvec, T_n);

    Eigen::Vector3f euler_angles = R_n.eulerAngles(0, 1, 2);

    float pitch = euler_angles[0] * 180 / 3.1416;
    float yaw = euler_angles[1] * 180 / 3.1416;
    float roll = euler_angles[2] * 180 / 3.1416;

    cout << setprecision(4) << "yaw: " << yaw << "pitch: " << pitch << "roll" << roll << endl;*/

    //输出距离
    cout << "距离" << Tvec.at<float>(2) << endl; 

    //尝试用两种方式求解三个轴偏转的角度
    //第一种
    cv::Mat pose_mat = cv::Mat(3, 4, CV_64FC1);

    cv::hconcat(rotMat, taux, pose_mat);

    cv::Mat euler_angle = cv::Mat(3, 1, CV_64FC1);
    getEulerAngles(pose_mat, euler_angle);

    //第二种
    if (isRotationMatrix(rotMat))
    {
        rotationMatrixToEulerAngles(rotMat);
    }
    else
    {
        cout << "not rotation matrix" << endl;
    }
}
#include "armordetection.h"

ArmorDetection::ArmorDetection() = default;

ArmorDetection::ArmorDetection(Mat &input)
{
    frame = input;
}

void ArmorDetection::setInputImage(Mat input)
{
    frame = input;
    currentCenter.x = 0;
    currentCenter.y = 0;
}

// 图像预处理
void ArmorDetection::Pretreatment()
{
    cvtColor(frame, hsv, COLOR_BGR2HSV);

    // 颜色范围参数
    Scalar lowerBound1(H_LOW_1, S_LOW, V_LOW);
    Scalar upperBound1(H_HIGH_1, S_HIGH, V_HIGH);

    // 创建红色掩膜
    inRange(hsv, lowerBound1, upperBound1, mask);

    // 形态学操作
    morphologyEx(mask, mask, MORPH_OPEN, kernel1, Point(-1, -1)); // 开操作
    dilate(mask, mask, kernel2, Point(-1, -1), 1);                // 膨胀
    // 轮廓增强
    Canny(mask, mask, 3, 9, 3);
    // 寻找轮廓
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(mask, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // 筛选，去除一部分矩形
    Point2f vertex[4];
    for (int i = 0; i < contours.size(); ++i)
    {
        RotatedRect minRect = minAreaRect(Mat(contours[i]));
        minRect.points(vertex);
        Vec3b hsvValue = hsv.at<Vec3b>(minRect.center.y, minRect.center.x);
        if ((minRect.size.width * 10 > minRect.size.height) &&
            (minRect.size.width * 2 < minRect.size.height) &&
            (hsvValue[2] > 220) &&
            (minRect.size.width * minRect.size.height > 2000))
        {
            minRects.push_back(minRect);
        }

        // if (minRects.size() > 3)
        // {
        //     // 对minRects按照面积进行排序（从小到大）
        //     std::sort(minRects.begin(), minRects.end(),
        //               [](const cv::RotatedRect &a, const cv::RotatedRect &b)
        //               {
        //                   return a.size.area() < b.size.area();
        //               });

        //     // 删除面积最小的那个元素，即第一个
        //     minRects.erase(minRects.begin());
        // }
        for (const auto &rect : minRects)
        {
            // 获取矩形的四个顶点
            Point2f vertices[4];
            rect.points(vertices);

            // 绘制矩形的四条边
            for (int l = 0; l < 4; l++)
            {
                line(frame, vertices[l], vertices[(l + 1) % 4], Scalar(0, 255, 0), 2);
            }
        }
    }
}

Point2f ArmorDetection::GetArmorCenter()
{
    // 遍历所有矩形，两两组合
    RotatedRect leftRect, rightRect;
    vector<int *> reliability;
    double area[2], distance, height;

    if (minRects.size() < 2)
    {
        LostTarget();
        return currentCenter;
    }

    for (int i = 0; i < minRects.size(); ++i)
    {
        for (int j = i + 1; j < minRects.size(); ++j)
        {
            int level = 0;
            int temp[3];
            leftRect = minRects[i];
            rightRect = minRects[j];

            // 角度
            if (abs(leftRect.angle - rightRect.angle) < 1)
                level += 20;
            else if (abs(leftRect.angle - rightRect.angle) < 2)
                level += 8;
            else if (abs(leftRect.angle - rightRect.angle) < 3)
                level += 6;
            else if (abs(leftRect.angle - rightRect.angle) < 4)
                level += 4;
            else if (abs(leftRect.angle - rightRect.angle) < 5)
                level += 2;
            else
                level += 0;

            // 中心点颜色
            Point2f centerP = Point2f((leftRect.center.x + rightRect.center.x) / 2, (leftRect.center.y + rightRect.center.y) / 2);
            Rect roi(centerP.x - 2, centerP.y - 2, 5, 5);
            Mat region = frame(roi);
            if (isNearlyWhite(region))
                level += 30;

            // 中心高度差
            double half_height = (leftRect.size.height + rightRect.size.height) / 4;
            if (abs(leftRect.center.y - rightRect.center.y) < 0.05 * half_height)
                level += 10;
            else if (abs(leftRect.center.y - rightRect.center.y) < 0.1 * half_height)
                level += 8;
            else if (abs(leftRect.center.y - rightRect.center.y) < 0.15 * half_height)
                level += 6;
            else if (abs(leftRect.center.y - rightRect.center.y) < 0.2 * half_height)
                level += 4;
            else if (abs(leftRect.center.y - rightRect.center.y) < 0.25 * half_height)
                level += 2;
            else
                level += 0;

            // 板长宽比
            distance = Distance(leftRect.center, rightRect.center);
            height = (leftRect.size.height + rightRect.size.height) / 2;
            if (distance != 0 && distance > height)
            {
                if (distance > 2 * height && distance < 2.4 * height)
                    level += 8;
                else
                {
                }
            }
            else
            {
            }

            temp[0] = i;
            temp[1] = j;
            temp[2] = level;

            reliability.push_back(temp);
        }
    }

    int maxLevel = 0, index = 0;
    for (int k = 0; k < reliability.size(); ++k)
    {
        if (reliability[k][2] > maxLevel)
        {
            maxLevel = reliability[k][2];
            index = k;
        }
    }

    currentCenter.x = (minRects[reliability[index][0]].center.x + minRects[reliability[index][1]].center.x) / 2;
    currentCenter.y = (minRects[reliability[index][0]].center.y + minRects[reliability[index][1]].center.y) / 2;

    line(frame, Point(currentCenter.x - 10, currentCenter.y - 10),
         Point(currentCenter.x + 10, currentCenter.y + 10), Scalar(255, 255, 0), 5);
    line(frame, Point(currentCenter.x + 10, currentCenter.y - 10),
         Point(currentCenter.x - 10, currentCenter.y + 10), Scalar(255, 255, 0), 5);
    circle(frame, currentCenter, 7.5, Scalar(0, 0, 255), 5);
    imshow("Video", frame);

    minRects.clear();
    double h = (minRects[reliability[index][1]].size.height +
                minRects[reliability[index][0]].size.height) / 4;
    double dx = h * abs(cos((minRects[reliability[index][1]].angle +
                minRects[reliability[index][0]].angle)/2));
    double dy = h * abs(sin((minRects[reliability[index][1]].angle +
                minRects[reliability[index][0]].angle)/2));
    tRects.clear();
    tRects.push_back(Point2f(minRects[reliability[index][1]].center.x - dx, minRects[reliability[index][1]].center.y - dy));
    tRects.push_back(Point2f(minRects[reliability[index][1]].center.x + dx, minRects[reliability[index][1]].center.y + dy));
    tRects.push_back(Point2f(minRects[reliability[index][0]].center.x + dx, minRects[reliability[index][0]].center.y + dy));
    tRects.push_back(Point2f(minRects[reliability[index][0]].center.x - dx, minRects[reliability[index][0]].center.y - dy));
    return currentCenter;
}

void ArmorDetection::LostTarget()
{
    lost++;
    if (lost < 3)
    {
        currentCenter = lastCenter;
    }
    else
    {
        currentCenter = Point2f(0, 0);
        lastCenter = Point2f(0, 0);
    }
}

double ArmorDetection::Distance(Point2f a, Point2f b)
{
    return sqrt((a.x - b.x) * (a.x - b.x) +
                (a.y - b.y) * (a.y - b.y));
}

double ArmorDetection::max(double first, double second)
{
    return first > second ? first : second;
}

double ArmorDetection::min(double first, double second)
{
    return first < second ? first : second;
}

bool ArmorDetection::isNearlyWhite(const Mat &region)
{
    // 定义白色在HSV颜色空间的阈值
    Scalar lowerBound(80, 0, 60);    // S_MIN, V_MIN for nearly white
    Scalar upperBound(120, 80, 100); // H_MAX, S_MAX, V_MAX for nearly white

    // 创建掩膜，找出接近白色的像素
    Mat mask1;
    inRange(region, lowerBound, upperBound, mask1);
    imshow("mask1", mask1);
    // 计算接近白色像素的比例
    double nonZeroCount = countNonZero(mask);
    double totalPixels = region.rows * region.cols;
    double whiteRatio = nonZeroCount / totalPixels;

    // 如果接近白色的比例大于某个阈值，则认为是白色
    const double WHITE_THRESHOLD = 0.8; // 可以调整此阈值
    return whiteRatio >= WHITE_THRESHOLD;
}

void ArmorDetection::coordinate(Point2f center, vector<Point2f> imagePoints)
{
    cv::Mat cameraMatrix = (cv::Mat_<double>(3, 3) << 458.654, 0.000, 367.215,
                            0.000, 457.296, 248.375,
                            0.000, 0.000, 1.000);
    // 畸变参数
    cv::Mat distCoeffs = (cv::Mat_<double>(1, 5) << -0.28340811, 0.07395907, 0.00019359, 1.76187114e-05, 0.0);

    std::vector<cv::Point3f> objectPoints;
    objectPoints.push_back(cv::Point3f(115.00F, -28.50F, 0));
    objectPoints.push_back(cv::Point3f(115.00F, 28.50F, 0));
    objectPoints.push_back(cv::Point3f(-115.00F, 28.50F, 0));
    objectPoints.push_back(cv::Point3f(-115.00F, -28.50F, 0));

    // 使用PnP算法估算物体的旋转和位移向量
    cv::Mat rvec, tvec;
    solvePnP(objectPoints, imagePoints, cameraMatrix, distCoeffs, rvec, tvec);

    double z = sqrt(pow(tvec.at<double>(0), 2) + pow(tvec.at<double>(1), 2) + pow(tvec.at<double>(2), 2)) / 1000.0;
    double u = center.x, v = center.y;
    double x = (u - cameraMatrix.at<double>(0, 2)) * z / cameraMatrix.at<double>(0, 0);
    double y = (v - cameraMatrix.at<double>(1, 2)) * z / cameraMatrix.at<double>(1, 1);

    // 将旋转向量 rvec 转换为旋转矩阵 R
    cv::Mat R;
    cv::Rodrigues(rvec, R);

    // 创建四元数
    Eigen::Quaterniond quaternion = Eigen::Quaterniond(R.at<double>(0, 0), R.at<double>(0, 1), R.at<double>(0, 2), R.at<double>(1, 0));
    quaternion.normalize(); // 四元数归一化

    cout << x << " " << y << " " << z << "  " << quaternion.w() << ", " << quaternion.x() << ", " << quaternion.y() << ", " << quaternion.z() << std::endl;;
}
#ifndef ARMORDETECTION_H
#define ARMORDETECTION_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <eigen3/Eigen/Dense>

using namespace std;
using namespace cv;

class ArmorDetection {
private:
	Mat frame, hsv, mask;
	Mat kernel1 = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	Mat kernel2 = getStructuringElement(MORPH_RECT, Size(5, 5), Point(-1, -1));
	Point2f currentCenter;
	Point2f lastCenter;
	vector<RotatedRect> minRects;
	int lost;

public:	
	vector<Point2f> tRects;

	ArmorDetection();
	explicit ArmorDetection(Mat& input);
	void setInputImage(Mat input);
	void Pretreatment();
	Point2f GetArmorCenter();
	bool isNearlyWhite(const Mat& region);
	void coordinate(Point2f center, vector<Point2f> rect);
	int H_LOW_1 = 80;
	int H_HIGH_1 = 120;
	int S_LOW = 0;
	// int H_LOW_1 = 0;
	// int H_HIGH_1 = 30;
	// int S_LOW = 50;

	int V_LOW = 150;
	int S_HIGH = 255;
	int V_HIGH = 255;

private:
	void LostTarget();
	double Distance(Point2f, Point2f);
	double max(double, double);
	double min(double, double);
};


#endif 

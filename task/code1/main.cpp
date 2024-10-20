#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "armordetection.h"

using namespace cv;
using namespace std;

ArmorDetection* armor = new ArmorDetection();
Point2f center;

int main()
{
	//fps变量
	double t = (double)getTickCount();
	double fps;
	char string[10];
	char string2[10];
	Mat frame;

	VideoCapture capture("C:/Users/Administrator/Desktop/video/6.mp4");
	if (!capture.isOpened())
	{
		printf("无法打开相机...\n");
		return -1;
	}

	namedWindow("frame", CV_WINDOW_AUTOSIZE);
	namedWindow("mask", CV_WINDOW_AUTOSIZE);
	namedWindow("Control", CV_WINDOW_AUTOSIZE);

	while (capture.read(frame))//读取当前帧
	{
		armor->setInputImage(frame);
		armor->Pretreatment();
		center = armor->GetArmorCenter();
		cout << "[INFO] x = " << center.x - frame.cols / 2 << "    y = " << center.y - frame.rows / 2 << endl;

		//计算fps
		double dt = ((double)getTickCount() - t) / (double)getTickFrequency();
		fps = 1.0 / dt;
		t = (double)getTickCount();
		sprintf_s(string, "%.2f", fps);
		std::string fpsString("FPS:");
		fpsString += string;
		putText(frame, fpsString, Point(5, 20), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 255, 255));

		char c = waitKey(10);
		if (c == 27) //"Esc"
		{
			break;
		}
	}
	capture.release();//释放视频内存
	waitKey(0);
	return 0;
}

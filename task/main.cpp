#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <iostream>
#include "armordetection.h"

using namespace std;
using namespace cv;

ArmorDetection *armor = new ArmorDetection();
Point2f center;

int main()
{

	// fps变量
	// double t = (double)getTickCount();
	// double fps;
	// char string[10];
	// char string2[10];

	VideoCapture cap("../video/video2.mp4"); // 获取视频
	if (!cap.isOpened())
	{
		cout << "Error: Could not open video." << endl;
		return -1;
	}

	namedWindow("Video", WINDOW_NORMAL);
	resizeWindow("Video", 2000, 1000);

	Mat frame;
	// int frame_width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
	// int frame_height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
	// double fps = cap.get(cv::CAP_PROP_FPS);

	// 创建VideoWriter对象
	// cv::VideoWriter writer("/home/manba/Project/task/process.mp4", cv::VideoWriter::fourcc('H','2','6','4'), fps, cv::Size(frame_width, frame_height), true);
	// if (!writer.isOpened()) {
	//     std::cerr << "Could not open the output video file for writing" << std::endl;
	//     return -1;
	// }

	while (cap.read(frame))
	{
		armor->setInputImage(frame);
		armor->Pretreatment();
		center = armor->GetArmorCenter();
		armor->coordinate(center,armor->tRects);

		// 计算fps
		// double dt = ((double)getTickCount() - t) / (double)getTickFrequency();
		// fps = 1.0 / dt;
		// t = (double)getTickCount();
		// sprintf_s(string, "%.2f", fps);
		// std::string fpsString("FPS:");
		// fpsString += string;
		// putText(frame, fpsString, Point(5, 20), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 255, 255));

		// writer.write(frame);
		if (waitKey(30) >= 0)
			break;
	}

	cap.release();
	// writer.release();
	destroyAllWindows();
}

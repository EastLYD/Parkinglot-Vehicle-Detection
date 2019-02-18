#include "opencv2/objdetect.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core/utility.hpp"

//#include "opencv2/videoio/videoio_c.h"
#include "opencv2/highgui/highgui_c.h"

#include <cctype>
#include <iostream>
#include <iterator>
#include <stdio.h>
#include<stdlib.h>

using namespace std;
using namespace cv;

int main()
{

	VideoCapture capture;
	
	capture.open("C:/Users/asus/Documents/Tencent Files/316774667/FileRecv/parking.avi");
	//capture = 0;
	if (!capture.isOpened())//判断是否打开视频文件  
	{
		exit(1);
	}


	Mat frame, bframe, curframe, result;
	namedWindow("播放界面按Esc退出", 0);
	cvResizeWindow("播放界面按Esc退出", 600, 500);
	while (true)
	{
		capture >> frame;
		if (frame.empty())break;
		if (bframe.empty())bframe = frame.clone();
		curframe = frame.clone();



		Mat result = frame.clone();
		Mat curimageROI = bframe(Rect(0, bframe.rows/8, bframe.cols , bframe.rows / 8));
		Mat preimageROI = curframe(Rect(0, curframe.rows /8, curframe.cols, curframe.rows / 8));

		Mat sm_pre, sm_cur;
		GaussianBlur(preimageROI, sm_pre, Size(13, 13), 2, 2);
		GaussianBlur(curimageROI, sm_cur, Size(13, 13), 2, 2);
		
		imshow("平滑处理", sm_cur);

		Mat gray_pre, gray_cur;
		cvtColor(sm_pre, gray_pre, CV_RGB2GRAY);
		cvtColor(sm_cur, gray_cur, CV_RGB2GRAY);
		imshow("灰度处理", gray_cur);

		Mat diff;
		absdiff(gray_pre, gray_cur, diff);
		imshow("帧差处理", diff);

		Mat ez;
		threshold(diff, ez, 30, 255, CV_THRESH_BINARY);
	
		imshow("二值化处理", ez);



		Mat pz;
		Mat element1 = getStructuringElement(MORPH_RECT, Size(11, 30));
		dilate(ez, pz, element1);
		imshow("膨胀处理", pz);
		Mat fs;
		Mat element2 = getStructuringElement(MORPH_RECT, Size(10, 16));
		erode(pz, fs, element2);
		imshow("腐蚀处理", fs);



		vector<vector<Point> > contours;
		findContours(fs, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		vector<Rect> boundRect(contours.size());
		for (int i = 0; i < contours.size(); i++)
		{
			boundRect[i] = boundingRect(contours[i]);
			//Rect(boundRect[i].x + frame.cols, boundRect[i].y + frame.rows, boundRect[i].width, boundRect[i].height)
			rectangle(result, Rect(boundRect[i].x , boundRect[i].y + frame.rows / 8, boundRect[i].width, boundRect[i].height), Scalar(0, 255, 0), 1);//在result上绘制正外接矩形  
		}

		imshow("播放界面按Esc退出", frame);
		imshow("识别界面", result);
		imshow("背景", bframe);
		if (waitKey(1000.0 / 120) == 27)//按原FPS显示  
		{
			cout << "ESC退出!" << endl;
			break;
		}
	}
	capture.release();
}  

#include "opencv2/objdetect.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core/utility.hpp"

//#include "opencv2/videoio/videoio_c.h"
#include "opencv2/highgui/highgui_c.h"

#include <cctype>
#include <iostream>
#include <iterator>
#include <stdio.h>
#include<stdlib.h>

#include <vector>

using namespace cv;
using std::vector;

int main() {
	VideoCapture capture("C:/Users/asus/Documents/Tencent Files/316774667/FileRecv/parking.avi");
	Mat frame, preframe, curframe, result;
	Mat img;
	namedWindow("播放界面", 0);
	while (true)
	{
		capture >> frame;
		if (frame.empty())break;
		if (preframe.empty())preframe = frame.clone();//首帧处理
		curframe = frame.clone();
		Mat preframeROI = preframe(Rect(0, 117, 1664, 200));
		Mat curframeROI = curframe(Rect(0, 117, 1664, 200));
		Mat preImg, curImg;

		GaussianBlur(preframeROI, preImg, Size(13, 13), 2, 2);
		GaussianBlur(curframeROI, curImg, Size(13, 13), 2, 2);
		cvtColor(preImg, preImg, CV_RGB2GRAY);
		cvtColor(curImg, curImg, CV_RGB2GRAY);
		absdiff(preImg, curImg, img);
		imshow("xx1", img);
		threshold(img, img, 30, 255, CV_THRESH_BINARY);//二值化处理，设定阈值为30，处理当前点值大于阈值时，取白色255
		imshow("xx2", img);
		Mat element = getStructuringElement(MORPH_RECT, Size(11, 30));
		erode(img, img, element);
		imshow("xx3", img);
		element = getStructuringElement(MORPH_RECT, Size(10, 16));
		dilate(img, img, element);
		imshow("xx4", img);

		vector <vector<Point>> contours;
		findContours(img, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		vector<Rect> boundRect(contours.size());
		for (int i = 0; i < contours.size(); i++)
		{
			boundRect[i] = boundingRect(contours[i]);
			//Rect(boundRect[i].x + frame.cols, boundRect[i].y + frame.rows, boundRect[i].width, boundRect[i].height)
			rectangle(result, Rect(boundRect[i].x + frame.cols / 5, boundRect[i].y + frame.rows / 5, boundRect[i].width, boundRect[i].height), Scalar(0, 255, 0), 1);
			//在result上绘制正外接矩形  
		}
	//	imshow("Test", result);


		waitKey(30);
		preframe = frame.clone();//记录当前帧为下一帧的前帧
	}
}
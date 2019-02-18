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
	if (!capture.isOpened())//�ж��Ƿ����Ƶ�ļ�  
	{
		exit(1);
	}


	Mat frame, bframe, curframe, result;
	namedWindow("���Ž��水Esc�˳�", 0);
	cvResizeWindow("���Ž��水Esc�˳�", 600, 500);
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
		
		imshow("ƽ������", sm_cur);

		Mat gray_pre, gray_cur;
		cvtColor(sm_pre, gray_pre, CV_RGB2GRAY);
		cvtColor(sm_cur, gray_cur, CV_RGB2GRAY);
		imshow("�Ҷȴ���", gray_cur);

		Mat diff;
		absdiff(gray_pre, gray_cur, diff);
		imshow("֡���", diff);

		Mat ez;
		threshold(diff, ez, 30, 255, CV_THRESH_BINARY);
	
		imshow("��ֵ������", ez);



		Mat pz;
		Mat element1 = getStructuringElement(MORPH_RECT, Size(11, 30));
		dilate(ez, pz, element1);
		imshow("���ʹ���", pz);
		Mat fs;
		Mat element2 = getStructuringElement(MORPH_RECT, Size(10, 16));
		erode(pz, fs, element2);
		imshow("��ʴ����", fs);



		vector<vector<Point> > contours;
		findContours(fs, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		vector<Rect> boundRect(contours.size());
		for (int i = 0; i < contours.size(); i++)
		{
			boundRect[i] = boundingRect(contours[i]);
			//Rect(boundRect[i].x + frame.cols, boundRect[i].y + frame.rows, boundRect[i].width, boundRect[i].height)
			rectangle(result, Rect(boundRect[i].x , boundRect[i].y + frame.rows / 8, boundRect[i].width, boundRect[i].height), Scalar(0, 255, 0), 1);//��result�ϻ�������Ӿ���  
		}

		imshow("���Ž��水Esc�˳�", frame);
		imshow("ʶ�����", result);
		imshow("����", bframe);
		if (waitKey(1000.0 / 120) == 27)//��ԭFPS��ʾ  
		{
			cout << "ESC�˳�!" << endl;
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
	namedWindow("���Ž���", 0);
	while (true)
	{
		capture >> frame;
		if (frame.empty())break;
		if (preframe.empty())preframe = frame.clone();//��֡����
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
		threshold(img, img, 30, 255, CV_THRESH_BINARY);//��ֵ�������趨��ֵΪ30������ǰ��ֵ������ֵʱ��ȡ��ɫ255
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
			//��result�ϻ�������Ӿ���  
		}
	//	imshow("Test", result);


		waitKey(30);
		preframe = frame.clone();//��¼��ǰ֡Ϊ��һ֡��ǰ֡
	}
}
#include "stdafx.h"
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <stdio.h>
#include <cmath>

using namespace std;
using namespace cv;



enum posC {E,NE,N,NW,W,SW,S,SE};
int currentDir=0;
Point2d retrieveNextB(Mat img, Point2d b);
Point2d retrieveNextC(Mat img, Point2d b);


int allContours(Mat binaryImage, vector< vector<Point> > & contourVecVec);


int main() {

	Mat frame = imread("C:/Users/Arthur/Documents/Moore/Moore/basisfiguren.jpg", CV_LOAD_IMAGE_COLOR);

	Mat gray_image;
	cvtColor(frame, gray_image, CV_BGR2GRAY);

	// Het tonen van grijswaarde beeld
	imshow("MyVideo", gray_image);

	Mat binaryImage;
	Mat binaryImageShow;
	threshold(gray_image, binaryImage, 200, 1, CV_THRESH_BINARY_INV);
	threshold(gray_image, binaryImageShow, 200, 200, CV_THRESH_BINARY_INV);
	imshow("threshold", binaryImageShow);


	Mat binary16S;
	Mat binary16SShow;
	binaryImage.convertTo(binary16S, CV_16S);
	binaryImageShow.convertTo(binary16SShow, CV_16S);
	imshow("binary", binary16SShow);

	vector< vector<Point> > *contourVecVec;

	allContours(binary16S, *contourVecVec);

	
	waitKey(27);
	while (true)
	{

	}

	return 0;
}

int allContours(Mat binaryImage, vector< vector<Point> > & contourVecVec)
{
	Mat labeledImage;

	vector<Point2d *> firstpixelVec2;
	vector<Point2d *> posVec2;
	vector<int> areaVec2;
	labelBLOBsInfo(binaryImage, labeledImage, firstpixelVec2, posVec2, areaVec2);


	Point2d *firstB0;
	Point2d firstC0;


	for (Point2d *ptr : firstpixelVec2)
	{
		vector<Point> obj;

		firstB0 = ptr;
		//Check if bgX is not 0?
		//c0 = Point2d(b0->x - 1, b0->y);
		firstC0 = retrieveNextC(labeledImage, *firstB0);


	}
	
}

Point2d retrieveNextC(Mat img, Point2d b)
{
	Point2d nextC;
	switch (currentDir)
	{
	case posC::LEFT:
		if (img.at<int>(b.x, b.y - 1) == 1)
		{
			currentDir++;
		}
		else
		{
			nextC = Point2d(b.x, b.y - 1);
			break;
		}
	case posC::TOP:
		if (img.at<int>(b.x+1, b.y) == 1)
		{
			currentDir++;
		}
		else
		{
			nextC = Point2d(b.x, b.y);
			break;
		}
	}
	
	
	if (currentDir != 3)
		currentDir++;
	else
		currentDir = 0;


}

Point2d retrieveNextB(Mat img, Point2d b)
{
	Point2d nextB;
	currentDir++;
	switch (currentDir)
	{
	case posC::E:
		if (img.at<int>(b.x-1, b.y) == 1)
		{
			nextB = Point2d(b.x-1, b.y );
			break;
		}
		else
		{
			currentDir++;
		}
	case posC::NE:
		if (img.at<int>(b.x-1, b.y - 1) == 1)
		{
			nextB = Point2d(b.x-1, b.y - 1);
			break;
		}
		else
		{
			currentDir++;
		}

	case posC::N:
		if (img.at<int>(b.x, b.y - 1) == 1)
		{
			nextB = Point2d(b.x, b.y - 1);
			break;
		}
		else
		{
			currentDir++;
		}

	case posC::NW:
		if (img.at<int>(b.x+1, b.y - 1) == 1)
		{
			nextB = Point2d(b.x+1, b.y - 1);
			break;
		}
		else
		{
			currentDir++;
		}
	case posC::W:
		if (img.at<int>(b.x+1, b.y) == 1)
		{
			nextB = Point2d(b.x+1, b.y);
			break;
		}
		else
		{
			currentDir++;
		}
	case posC::SW:
		if (img.at<int>(b.x+1, b.y + 1) == 1)
		{
			nextB = Point2d(b.x+1, b.y - 1);
			break;
		}
		else
		{
			currentDir++;
		}
	case posC::S:
		if (img.at<int>(b.x, b.y + 1) == 1)
		{
			nextB = Point2d(b.x, b.y + 1);
			break;
		}
		else
		{

			currentDir++;
		}
	case posC::SE:
		if (img.at<int>(b.x-1, b.y + 1) == 1)
		{
			nextB = Point2d(b.x, b.y - 1);
			break;
		}
		else
		{
			return nullptr;
		}
	

	}



}




#include "stdafx.h"
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <stdio.h>
#include <cmath>

using namespace std;
using namespace cv;

int allContours(Mat binaryImage, vector< vector<Point> > & contourVecVec);
void findNextB(Mat img, Point p, vector<Point> vec);
bool containsPoint(vector<Point> points, Point p);


Point getPoint(Point p, int dirNr);

int getDirectionClosest(Mat img, Point p, int startDir = 0);

int main() {

	Mat frame = imread("C:/Users/Arthur/Documents/SourceTree/Vision-Cpp-voor-gevorderden/VisionCPP/MooreV2/test.png", CV_LOAD_IMAGE_COLOR);

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

	vector< vector<Point> > *contourVecVec = new vector<vector<Point>>();

	allContours(binary16S, *contourVecVec);

	for (vector<Point> vec : *contourVecVec)
	{
		for (Point e : vec)
			circle(frame, e, 3, Scalar(128, 128, 128));
	}


	waitKey(27);
	while (true)
	{

	}

	return 0;


	
}

int allContours(Mat binaryImage, vector< vector<Point> > & contourVecVec) 
{

	Mat labeledImage;
	Mat C = (Mat_<double>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
	/*binaryImage = (Mat_<double>(6, 6) << 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 0, 0,
		0, 1, 0, 0, 1, 0,
		0, 0, 1, 0, 1, 0,
		0, 1, 0, 0, 1, 0,
		0, 1, 1, 1, 0, 0);*/

	vector<Point2d *> firstpixelVec2;
	vector<Point2d *> posVec2;
	vector<int> areaVec2;
	labelBLOBsInfo(binaryImage, labeledImage, firstpixelVec2, posVec2, areaVec2);

	std::cout << binaryImage << std::endl;

	Point2d *firstB0;
	//Point2d firstC0;


	for (Point2d *ptr : firstpixelVec2)
	{
		vector<Point> obj;
		
		firstB0 = ptr;
		//Check if bgX is not 0?
		//c0 = Point2d(b0->x - 1, b0->y);
		//firstC0 = retrieveNextC(labeledImage, *firstB0);


		Point2d b = Point(firstB0->x, firstB0->y);
		obj.push_back(b);

		findNextB(binaryImage, b, obj);
		

	}

	return 0;
}

void findNextB(Mat img, Point p, vector<Point> vec)
{
	int nearest;
	int newX = p.x;
	int newY = p.y;
	//findNext1(makeAdmin(img), newX, newY, nearest);
	//Point nextPoint(newX, newY);
	int dir;
	Point nextPoint;
	int currentDir = 0;
	bool looping = true;

	while (looping)
	{

		dir = getDirectionClosest(img, p,currentDir);
		nextPoint = getPoint(p, dir);

		if (!containsPoint(vec, nextPoint))
		{
			looping = false;
		}
		else
		{
			if (!currentDir < 8)
				currentDir++;
			else
				currentDir = 0;
		}
	}


	//Point nextPoint = getPoint(p, nearest);
	

	if (!containsPoint(vec, nextPoint))
	{
		vec.push_back(nextPoint);
		findNextB(img, nextPoint, vec);

	}
	else
	{

	}
	return;
}

bool containsPoint(vector<Point> points, Point p)
{
	for (Point e : points)
	{
		if (e.x == p.x && e.y == p.y)
		{
			return true;
		}
	}
	return false;
}

int getDirectionClosest(Mat img, Point p,int startDir)
{
	for (int i = startDir; i < 8; i++)
	{
		Point newPoint = getPoint(p,i);
		int val = getEntryImage(img, newPoint.y, newPoint.x);
		if (val==1)
		{
			
			return i;
		}
	}
	return -1;
}

Point getPoint(Point p, int dirNr)
{
	Point newPoint(0,0);
	switch (dirNr)
	{
	case 0:
		newPoint = Point(p.x-1, p.y);
		break;
	case 1:
		newPoint = Point(p.x-1, p.y -1);
		break;
	case 2:
		newPoint = Point(p.x, p.y-1);
		break;
	case 3:
		newPoint = Point(p.x+1, p.y - 1);
		break;
	case 4:
		newPoint = Point(p.x+1, p.y);
		break;
	case 5:
		newPoint = Point(p.x+1, p.y + 1);
		break;
	case 6:
		newPoint = Point(p.x, p.y+1);
		break;
	case 7:
		newPoint = Point(p.x-1, p.y + 1);
		break;

	}

	return newPoint;
}



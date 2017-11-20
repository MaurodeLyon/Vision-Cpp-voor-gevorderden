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
int currentDir = 0;

Point getPoint(Point p, int dirNr);

int getDirectionClosest(Mat img, Point p, int startDir = 0);

Point2d *firstB0;

int main() {

	Mat frame = imread("C:/Users/Arthur/Documents/SourceTree/Vision-Cpp-voor-gevorderden/VisionCPP/MooreV2/test2.png", CV_LOAD_IMAGE_COLOR);

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

	
	//Point2d firstC0;


	for (Point2d *ptr : firstpixelVec2)
	{
		vector<Point> obj;
		currentDir = 0;
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

///Recursive function that finds the next 'b' based on the given point
///TODO correct 'c' tracking
void findNextB(Mat img, Point p, vector<Point> vec)
{
	int nearest;
	int newX = p.x;
	int newY = p.y;
	//findNext1(makeAdmin(img), newX, newY, nearest);
	//Point nextPoint(newX, newY);
	int dir;
	Point nextPoint;
	//int currentDir = 0;
	bool looping = true;

	while (looping)
	{
		//Retrieves direction to new 'b', currentDir is always the last dir minus 1 (eg dir 7 means currentDir 6, 0 means 7 etc)
		dir = getDirectionClosest(img, p,currentDir);
		//Retrieves coords of new 'b'
		nextPoint = getPoint(p, dir);

		//Checks if new point isnt already added and that dir has indeed found a 1 (-1 means an incorrect direction or none was found)
		// a -1 or already added point prompts the statement below to cycle to the next direction and check again
		if (!containsPoint(vec, nextPoint)&& dir!=-1)
		{
			//Stop while loop if new b is found
			looping = false;
			//Make sure currentDir doesnt go below 0
			if(dir>0)
				currentDir = dir-1;
			/*if (currentDir < 0)
			{
				currentDir = 8 - currentDir;
			}*/

		}
		else
		{
			if (currentDir < 7)
				currentDir++;
			else
				currentDir = 0;
		}
		//Check if origin is reached, if so abort
		if (nextPoint.x == firstB0->x&&nextPoint.y == firstB0->y)
		{
			return;
		}

		//if (dir == -1)
			//currentDir = 0;
	}


	//Point nextPoint = getPoint(p, nearest);
	
	//TODO: remove this check, should not be needed as this is already being checked in the aforementioned while loop
	if (!containsPoint(vec, nextPoint))
	{
		vec.push_back(nextPoint);
		findNextB(img, nextPoint, vec);

	}
	
	return;
}
///Because the == operator is not implemented for Mat objects this function is used.
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
///Gets closest 1 by turning clockwise from starting position until its found
///Returns -1 if no 1 was found in range.
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



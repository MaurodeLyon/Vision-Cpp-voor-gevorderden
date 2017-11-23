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
void findNextB(Mat img, Point b, Point c, vector<Point> &vec);
int getDirNr(Point p, Point c);
bool containsPoint(vector<Point> points, Point p);
//int currentDir = 0;

Point getPoint(Point p, int dirNr);

int getDirectionClosest(Mat img, Point p, int startDir = 0);
//Mat *contours;

Point2d *firstB0;

int main() {

	Mat frame = imread("C:/Users/Arthur/Documents/SourceTree/Vision-Cpp-voor-gevorderden/VisionCPP/MooreV2/basisfiguren.jpg", CV_LOAD_IMAGE_COLOR);

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

	Mat test = Mat::zeros(frame.rows, frame.cols, CV_64F);
	

	Mat contours = binaryImage.clone();
	contours = 0;
	
	//drawContours(test, *contourVecVec,-1, Scalar(128, 128, 128));
	imshow("done",test);
	/*for (vector<Point> vec : *contourVecVec)
	{
		for (Point e : vec)
			circle(frame, e, 3, Scalar(128, 128, 128));
	}*/

	for (vector<Point> vec : *contourVecVec)
	{
		for (Point e : vec)
			contours.at<uchar>(Point(e.x,e.y)) = 255;
	}

	imshow("testtesttest", contours);


	
	while (true)
	{
		waitKey(27);

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

	//std::cout << binaryImage << std::endl;

	
	//Point2d firstC0;


	for (Point2d *ptr : firstpixelVec2)
	{
		std::cout << "------" << std::endl;
		std::cout << "Point: " << ptr->x << " - " << ptr->y << std::endl;
		std::cout << getEntryImage(binaryImage, ptr->x, ptr->y);
		std::cout << "------" << std::endl;
		
		//To continue testing I had to exclude the last figure (due to one line thick figure, doesnt work as of now)
		if (ptr->x == 284 && ptr->y == 161)
		{
			return -1;
		}
		

		vector<Point> obj;
		//currentDir = 0;
		firstB0 = ptr;
		//Check if bgX is not 0?
		//c0 = Point2d(b0->x - 1, b0->y);
		//firstC0 = retrieveNextC(labeledImage, *firstB0);

		// Switching x and y because Jan's conventions are unconventional
		Point2d b = Point(firstB0->y, firstB0->x);
		
		std::cout << getEntryImage(binaryImage, b.y, b.x) << std::endl;
		obj.push_back(b);
		//TODO: This could fail if blob is at the edge of the image
		
		findNextB(binaryImage, b, Point(b.x-1,b.y), obj);
		
		contourVecVec.push_back(obj);

		std::cout << "contour done" << std::endl;

	}

	return 0;
}

///Recursive function that finds the next 'b' based on the given point
///TODO correct 'c' tracking
void findNextB(Mat img, Point b, Point c, vector<Point> &vec)
{
	int nearest;
	int newX = b.x;
	int newY = b.y;
	//findNext1(makeAdmin(img), newX, newY, nearest);
	//Point nextPoint(newX, newY);
	int dir;
	Point nextPoint;
	//Find currentDirectionNumber based on point c, relative to the latest 'b'
	int currentDir = getDirNr(b,c);
	bool looping = true;

	while (looping)
	{
		//Retrieves direction to new 'b', currentDir is always the last dir minus 1 (eg dir 7 means currentDir 6, 0 means 7 etc)
		dir = getDirectionClosest(img, b,currentDir);
		//Retrieves coords of new 'b'
		nextPoint = getPoint(b, dir);

		//Checks if new point isnt already added and that dir has indeed found a 1 (-1 means an incorrect direction or none was found)
		// a -1 or already added point prompts the statement below to cycle to the next direction and check again
		if (!containsPoint(vec, nextPoint)&& dir!=-1)
		{
			//Stop while loop if new b is found
			looping = false;
			//Make sure currentDir doesnt go below 0
			if (dir > 0)
				currentDir = dir - 1;
			else if (dir == 0)
				currentDir = 7;
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
		if (nextPoint.x == firstB0->y&&nextPoint.y == firstB0->x)
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
		//Calculate the coords of the last 0 preceding nextPoint
		Point newC = getPoint(b, currentDir);
		std::cout << "pointC: " << newC << std::endl;
		std::cout << "pointB: " << nextPoint << std::endl;
		vec.push_back(nextPoint);
		//contours.at<uchar>(Point(nextPoint.y, nextPoint.x)) = 255;
		findNextB(img, nextPoint, newC, vec);
		

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

int getDirNr(Point p, Point c)
{
	if (c.x == p.x - 1 && c.y == p.y)
		return 0;
	else if (c.x == p.x - 1 && c.y == p.y - 1)
		return 1;
	else if (c.x == p.x && c.y == p.y - 1)
		return 2;
	else if (c.x == p.x + 1 && c.y == p.y - 1)
		return 3;
	else if (c.x == p.x + 1 && c.y == p.y)
		return 4;
	else if (c.x == p.x + 1 && c.y == p.y + 1)
		return 5;
	else if (c.x == p.x && c.y == p.y + 1)
		return 6;
	else if (c.x == p.x - 1 && c.y == p.y + 1)
		return 7;

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



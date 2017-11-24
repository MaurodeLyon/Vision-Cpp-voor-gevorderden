#include "stdafx.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <stdio.h>
#include <cmath>

using namespace std;
using namespace cv;

int allContours(Mat image_binary, vector<vector<Point>>& contours);
void findNextB(Mat image, Point b, Point c, vector<Point>& contour);
int getDirNr(Point p, Point c);
bool containsPoint(vector<Point> contour, Point p);
Point getPoint(Point p, int dirNr);
int getDirectionClosest(Mat img, Point p, int startDir = 0);
double bendingEnergy(Mat binaryImage, vector<Point>& contourVec);

Point2d* firstB0;

int main()
{
	Mat image_original = imread("./../Images/square.bmp ", CV_LOAD_IMAGE_COLOR);
	imshow("Original", image_original);
	waitKey(0);

	Mat image_gray;
	cvtColor(image_original, image_gray, CV_BGR2GRAY);
	imshow("Gray", image_gray);
	waitKey(0);

	Mat image_binary;
	threshold(image_gray, image_binary, 200, 1, CV_THRESH_BINARY_INV);
	imshow("Binary", image_binary);
	waitKey(0);

	Mat image_binary16S;
	image_binary.convertTo(image_binary16S, CV_16S);
	show16SImageStretch(image_binary16S, "Binary 16S");
	waitKey(0);

	vector<vector<Point>>* contours = new vector<vector<Point>>();
	allContours(image_binary16S, *contours);
	waitKey(0);
}

int allContours(Mat image_binary, vector<vector<Point>>& contours)
{
	Mat labeledImage;
	Mat C = (Mat_<double>(3, 3) << 0 , -1 , 0 , -1 , 5 , -1 , 0 , -1 , 0);
	vector<Point2d *> firstpixelVec2;
	vector<Point2d *> posVec2;
	vector<int> areaVec2;
	labelBLOBsInfo(image_binary, labeledImage, firstpixelVec2, posVec2, areaVec2);

	for (Point2d* ptr : firstpixelVec2)
	{
		cout << "------" << endl;
		cout << "Point: " << ptr->x << " - " << ptr->y << endl;
		cout << getEntryImage(image_binary, ptr->x, ptr->y);
		cout << "------" << endl;

		//To continue testing I had to exclude the last figure (due to one line thick figure, doesnt work as of now)
		if (ptr->x == 284 && ptr->y == 161)
		{
			return -1;
		}
		vector<Point> obj;
		firstB0 = ptr;

		// Switching x and y because Jan's conventions are unconventional
		Point2d b = Point(firstB0->y, firstB0->x);
		cout << getEntryImage(image_binary, b.y, b.x) << endl;
		obj.push_back(b);
		findNextB(image_binary, b, Point(b.x - 1, b.y), obj);
		contours.push_back(obj);
		cout << "contour done" << endl;
	}
	return 0;
}

//Recursive function that finds the next 'b' based on the given point
//TODO correct 'c' tracking
void findNextB(Mat image, Point b, Point c, vector<Point>& contour)
{
	Point nextPoint;
	//Find currentDirectionNumber based on point c, relative to the latest 'b'
	int currentDir = getDirNr(b, c);
	bool looping = true;
	while (looping)
	{
		//Retrieves direction to new 'b', currentDir is always the last dir minus 1 (eg dir 7 means currentDir 6, 0 means 7 etc)
		int dir = getDirectionClosest(image, b, currentDir);

		//Retrieves coords of new 'b'
		nextPoint = getPoint(b, dir);

		//Checks if new point isnt already added and that dir has indeed found a 1 (-1 means an incorrect direction or none was found)
		// a -1 or already added point prompts the statement below to cycle to the next direction and check again
		if (!containsPoint(contour, nextPoint) && dir != -1)
		{
			//Stop while loop if new b is found
			looping = false;
			//Make sure currentDir doesnt go below 0
			if (dir > 0)
				currentDir = dir - 1;
			else if (dir == 0)
				currentDir = 7;
		}
		else
		{
			if (currentDir < 7)
				currentDir++;
			else
				currentDir = 0;
		}
		//Check if origin is reached, if so abort
		if (nextPoint.x == firstB0->y && nextPoint.y == firstB0->x)
		{
			return;
		}
	}

	//TODO: remove this check, should not be needed as this is already being checked in the aforementioned while loop
	if (!containsPoint(contour, nextPoint))
	{
		//Calculate the coords of the last 0 preceding nextPoint
		Point newC = getPoint(b, currentDir);
		cout << "pointC: " << newC << endl;
		cout << "pointB: " << nextPoint << endl;
		contour.push_back(nextPoint);
		findNextB(image, nextPoint, newC, contour);
	}
}

/*
 * Checks if a contour contains a point
 */
bool containsPoint(vector<Point> contour, Point point)
{
	for (Point p : contour)
	{
		if (p.x == point.x && p.y == point.y)
		{
			return true;
		}
	}
	return false;
}

/*
 * Turns clockwise around a point in a binary image to find a 1 based on a starting direction
 * returns -1 if no 1 was found in range
 */
int getDirectionClosest(Mat binary_image, Point point, int staring_direction)
{
	for (int i = staring_direction; i < 8; i++)
	{
		Point newPoint = getPoint(point, i);
		if (getEntryImage(binary_image, newPoint.y, newPoint.x) == 1)
		{
			return i;
		}
	}
	return -1;
}

/*
 * returns a direction number based on 2 points
 *    1 2 3
 *    0 p 4
 *    7 6 5
 * returns -1 if points are incorrect
 */
int getDirNr(Point p, Point c)
{
	if (c.x == p.x - 1 && c.y == p.y)
		return 0;
	if (c.x == p.x - 1 && c.y == p.y - 1)
		return 1;
	if (c.x == p.x && c.y == p.y - 1)
		return 2;
	if (c.x == p.x + 1 && c.y == p.y - 1)
		return 3;
	if (c.x == p.x + 1 && c.y == p.y)
		return 4;
	if (c.x == p.x + 1 && c.y == p.y + 1)
		return 5;
	if (c.x == p.x && c.y == p.y + 1)
		return 6;
	if (c.x == p.x - 1 && c.y == p.y + 1)
		return 7;

	return -1;
}

/*
 * returns a new point based on a current point and a direction
 *    1 2 3
 *    0 p 4
 *    7 6 5
 * returns Point(-1,-1) if a incorrect direction is supplied
 */
Point getPoint(Point p, int direction_number)
{
	switch (direction_number)
	{
	case 0:
		return Point(p.x - 1, p.y);
	case 1:
		return Point(p.x - 1, p.y - 1);
	case 2:
		return Point(p.x, p.y - 1);
	case 3:
		return Point(p.x + 1, p.y - 1);
	case 4:
		return Point(p.x + 1, p.y);
	case 5:
		return Point(p.x + 1, p.y + 1);
	case 6:
		return Point(p.x, p.y + 1);
	case 7:
		return Point(p.x - 1, p.y + 1);
	default:
		return Point(-1, -1);
	}
}

double bendingEnergy(Mat binaryImage, vector<Point>& contourVec)
{
	//Resolution scaling, seems to go out of bounds even though parameters suggest otherwise?

	/*Mat microImg = Mat::zeros(image_binary.rows/10, image_binary.cols/10, CV_8U);
	for (int rows = 0; rows < image_binary.rows; rows+=10)
	{
	for (int cols = 0; cols < image_binary.cols; cols+=10)
	{
	vector<int> pixels;
	for (int blockY = 0; blockY < 10; blockY++)
	{
	for (int blockX = 0; blockX < 10; blockX++)
	{
	if(image_binary.rows>(rows+blockY) && image_binary.cols>(cols+blockX))
	pixels.push_back(getEntryImage(image_binary, rows+blockY, cols+blockX));
	}
	}

	int sum = 0;
	for (int pix : pixels)
	{
	sum += pix;
	}
	int answer = sum / pixels.size();
	if(answer < 255)
	microImg.at<uchar>(Point(rows/10, cols/10))=answer;
	else
	microImg.at<uchar>(Point(rows / 10, cols / 10)) = 255;
	}
	}
	imshow("immabetesting", microImg);*/

	vector<int> chainCode;
	for (int i = 0; i < contourVec.size(); i++)
	{
		if (i + 1 < contourVec.size())
			chainCode.push_back(getDirNr(contourVec[i], contourVec[i + 1]));
		else
			chainCode.push_back(getDirNr(contourVec[i], contourVec[0]));
	}
	double bendingSum = 0;
	for (int i = 1; i < chainCode.size(); i++)
	{
		if (i + 1 < contourVec.size())
		{
			bendingSum += abs(chainCode[i] - chainCode[i - 1]);
		}

		else
		{
			bendingSum += abs(chainCode[i] - chainCode[0]);
		}
	}
	return bendingSum;
}

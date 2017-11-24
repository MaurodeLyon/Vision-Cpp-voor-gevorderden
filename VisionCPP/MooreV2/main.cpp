#include "stdafx.h"
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace std;
using namespace cv;

int allContours(Mat binaryImage, vector<vector<Point>>& contourVecVec);
void findNextB(Mat img, Point b, Point c, vector<Point> vec);
int getDirNr(Point p, Point c);
bool containsPoint(vector<Point> points, Point p);
//int currentDir = 0;

Point getPoint(Point p, int dirNr);

int getDirectionClosest(Mat img, Point p, int startDir = 0);

Point2d* firstB0;

int main()
{
	Mat image_original = imread("./../Images/basisfiguren.jpg", CV_LOAD_IMAGE_COLOR);
	imshow("Original", image_original);
	waitKey(0);

	Mat image_gray;
	cvtColor(image_original, image_gray, CV_BGR2GRAY);
	imshow("Gray", image_gray);
	waitKey(0);

	Mat image_binary;
	Mat image_binary16S;
	vector<vector<Point>>* contours = new vector<vector<Point>>();
	threshold(image_gray, image_binary, 200, 1, CV_THRESH_BINARY_INV);
	image_binary.convertTo(image_binary16S, CV_16S);
	allContours(image_binary16S, *contours);
	waitKey(0);

	Mat image_binary16s_show;
	Mat image_binary_show;
	threshold(image_gray, image_binary_show, 200, 200, CV_THRESH_BINARY_INV);
	image_binary_show.convertTo(image_binary16s_show, CV_16S);
	imshow("threshold", image_binary_show);
	imshow("binary", image_binary16s_show);
	waitKey(0);
}

int allContours(Mat binaryImage, vector<vector<Point>>& contours)
{
	Mat labeledImage;
	vector<Point2d *> firstPixels;
	vector<Point2d *> positions;
	vector<int> areas;
	labelBLOBsInfo(binaryImage, labeledImage, firstPixels, positions, areas);
	for (Point2d* first_pixel_point : firstPixels)
	{
		cout << "------" << endl;
		cout << "Point: " << first_pixel_point->x << " - " << first_pixel_point->y << endl;
		cout << getEntryImage(binaryImage, first_pixel_point->x, first_pixel_point->y);
		cout << "------" << endl;

		//To continue testing I had to exclude the last figure (due to one line thick figure, doesnt work as of now)
		if (first_pixel_point->x == 284 && first_pixel_point->y == 161)
		{
			return -1;
		}

		vector<Point> new_countour;
		firstB0 = first_pixel_point;

		// Switching x and y because Jan's conventions are unconventional
		Point2d correct_point = Point(firstB0->y, firstB0->x);
		cout << getEntryImage(binaryImage, correct_point.y, correct_point.x) << endl;
		new_countour.push_back(correct_point);
		findNextB(binaryImage, correct_point, Point(correct_point.x - 1, correct_point.y), new_countour);
		contours.push_back(new_countour);
		cout << "contour done" << endl;
	}
	return 0;
}

///Recursive function that finds the next 'b' based on the given point
///TODO correct 'current_point' tracking
void findNextB(Mat image, Point b, Point c, vector<Point> vec)
{
	int nearest;
	int newX = b.x;
	int newY = b.y;
	//findNext1(makeAdmin(image), newX, newY, nearest);
	//Point nextPoint(newX, newY);
	int dir;
	Point nextPoint;
	//Find currentDirectionNumber based on point current_point, relative to the latest 'b'
	int currentDir = getDirNr(b, c);
	bool looping = true;

	while (looping)
	{
		//Retrieves direction to new 'b', currentDir is always the last dir minus 1 (eg dir 7 means currentDir 6, 0 means 7 etc)
		dir = getDirectionClosest(image, b, currentDir);
		//Retrieves coords of new 'b'
		nextPoint = getPoint(b, dir);

		//Checks if new point isnt already added and that dir has indeed found a 1 (-1 means an incorrect direction or none was found)
		// a -1 or already added point prompts the statement below to cycle to the next direction and check again
		if (!containsPoint(vec, nextPoint) && dir != -1)
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
		if (nextPoint.x == firstB0->y && nextPoint.y == firstB0->x)
		{
			return;
		}

		//if (dir == -1)
		//currentDir = 0;
	}


	//Point nextPoint = getPoint(point, nearest);

	//TODO: remove this check, should not be needed as this is already being checked in the aforementioned while loop
	if (!containsPoint(vec, nextPoint))
	{
		//Calculate the coords of the last 0 preceding nextPoint
		Point newC = getPoint(b, currentDir);
		cout << "pointC: " << newC << endl;
		cout << "pointB: " << nextPoint << endl;
		vec.push_back(nextPoint);
		//contours.at<uchar>(Point(nextPoint.y, nextPoint.x)) = 255;
		findNextB(image, nextPoint, newC, vec);
	}
}

///Because the == operator is not implemented for Mat objects this function is used.
bool containsPoint(vector<Point> points, Point point)
{
	for (Point e : points)
	{
		if (e.x == point.x && e.y == point.y)
		{
			return true;
		}
	}
	return false;
}

///Gets closest 1 by turning clockwise from starting position until its found
///Returns -1 if no 1 was found in range.
int getDirectionClosest(Mat image, Point point, int start_direction)
{
	for (int i = start_direction; i < 8; i++)
	{
		Point newPoint = getPoint(point, i);
		int val = getEntryImage(image, newPoint.y, newPoint.x);
		if (val == 1)
		{
			return i;
		}
	}
	return -1;
}

int getDirNr(Point point, Point current_point)
{
	if (current_point.x == point.x - 1 && current_point.y == point.y)
		return 0;
	if (current_point.x == point.x - 1 && current_point.y == point.y - 1)
		return 1;
	if (current_point.x == point.x && current_point.y == point.y - 1)
		return 2;
	if (current_point.x == point.x + 1 && current_point.y == point.y - 1)
		return 3;
	if (current_point.x == point.x + 1 && current_point.y == point.y)
		return 4;
	if (current_point.x == point.x + 1 && current_point.y == point.y + 1)
		return 5;
	if (current_point.x == point.x && current_point.y == point.y + 1)
		return 6;
	if (current_point.x == point.x - 1 && current_point.y == point.y + 1)
		return 7;

	return -1;
}

Point getPoint(Point point, int direction)
{
	Point newPoint(0, 0);
	switch (direction)
	{
	case 0:
		newPoint = Point(point.x - 1, point.y);
		break;
	case 1:
		newPoint = Point(point.x - 1, point.y - 1);
		break;
	case 2:
		newPoint = Point(point.x, point.y - 1);
		break;
	case 3:
		newPoint = Point(point.x + 1, point.y - 1);
		break;
	case 4:
		newPoint = Point(point.x + 1, point.y);
		break;
	case 5:
		newPoint = Point(point.x + 1, point.y + 1);
		break;
	case 6:
		newPoint = Point(point.x, point.y + 1);
		break;
	case 7:
		newPoint = Point(point.x - 1, point.y + 1);
		break;
	}

	return newPoint;
}

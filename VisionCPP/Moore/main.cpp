#include "stdafx.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace std;
using namespace cv;

double bendingEnergy(Mat binaryImage, vector<Point>& contour);
void findNextB(Mat image, Point b, Point c, vector<Point> vec);
int allContours(Mat image_binary, vector<vector<Point>>& contours);
int getDirNr(Point p, Point c);
int getDirectionClosest(Mat image, Point p, int starting_direction = 0);
bool containsPoint(vector<Point> points, Point p);
Point getPoint(Point p, int direction_number);

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
	vector<Point2d *> firstPixels;
	vector<Point2d *> positions;
	vector<int> areas;
	labelBLOBsInfo(image_binary, labeledImage, firstPixels, positions, areas);
	for (Point2d* first_pixel_point : firstPixels)
	{
		cout << "------" << endl;
		cout << "Point: " << first_pixel_point->x << " - " << first_pixel_point->y << endl;
		cout << getEntryImage(image_binary, first_pixel_point->x, first_pixel_point->y) << endl;
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
		cout << getEntryImage(image_binary, correct_point.y, correct_point.x) << endl;
		new_countour.push_back(correct_point);
		findNextB(image_binary, correct_point, Point(correct_point.x - 1, correct_point.y), new_countour);
		contours.push_back(new_countour);
		cout << "contour done" << endl;
	}
	return 0;
}

double bendingEnergy(Mat binaryImage, vector<Point>& contour)
{
	double bending_energy = 0;


	return bending_energy;
}

//Recursive function that finds the next 'b' based on the given point
//TODO correct 'current_point' tracking
void findNextB(Mat image, Point b, Point c, vector<Point> vec)
{
	//Find currentDirectionNumber based on point current_point, relative to the latest 'b'
	int currentDir = getDirNr(b, c);
	bool looping = true;
	while (looping)
	{
		//Retrieves direction to new 'b', currentDir is always the last dir minus 1 (eg dir 7 means currentDir 6, 0 means 7 etc)
		int dir = getDirectionClosest(image, b, currentDir);
		//Retrieves coords of new 'b'
		Point nextPoint = getPoint(b, dir);

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
}

//Because the == operator is not implemented for Mat objects this function is used.
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

//Gets closest 1 by turning clockwise from starting position until its found
//Returns -1 if no 1 was found in range.
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

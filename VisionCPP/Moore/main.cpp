#include "stdafx.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <stdio.h>
#include <cmath>

using namespace std;
using namespace cv;

int getContours(Mat image_binary, vector<vector<Point>>& contours);
void findNextContourPixel(Mat image_binary, Point b, Point c, vector<Point>& contour, Point2d* origin);
int getDirectionNumber(Point p1, Point p2);
bool containsPoint(vector<Point> contour, Point p);
Point getDirectionPoint(Point p, int direction_number);
int getDirectionClosest(Mat image_binary, Point p, int starting_direction = 0);
double bendingEnergy(Mat image_binary, vector<Point>& contour);

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
	getContours(image_binary16S, *contours);
	waitKey(0);
}

/*
 * finds all contours in an image_binary
 */
int getContours(Mat image_binary, vector<vector<Point>>& contours)
{
	Mat image_labled;
	vector<Point2d *> firstPixels;
	vector<Point2d *> centers;
	vector<int> areas;
	labelBLOBsInfo(image_binary, image_labled, firstPixels, centers, areas);

	for (Point2d* firstPixel : firstPixels)
	{
		cout << "Getting contour for figure with first pixel ["
			<< firstPixel->x << "," << firstPixel->y << "] = " << getEntryImage(image_binary, firstPixel->x, firstPixel->y)
			<< endl;

		//To continue testing I had to exclude the last figure (due to one line thick figure, doesnt work as of now)
		//TODO: fix this problem
		if (firstPixel->x == 284 && firstPixel->y == 161)
		{
			return -1;
		}

		// Start creating vector which will contain the contour
		vector<Point> contour;
		Point2d starting_pixel = Point(firstPixel->y, firstPixel->x);
		contour.push_back(starting_pixel);
		findNextContourPixel(image_binary, starting_pixel, Point(starting_pixel.x - 1, starting_pixel.y), contour,
		                     firstPixel);
		contours.push_back(contour);
		cout << "contour done" << endl;
	}
	return 0;
}

/*
 * Recursive functions that looks for the next pixel 
 * it does this until it finds its way back to the origin
 */
void findNextContourPixel(Mat image, Point b, Point c, vector<Point>& contour, Point2d* origin)
{
	Point next_point;

	//Find currentDirectionNumber based on point p2, relative to the latest 'b'
	int current_direction = getDirectionNumber(b, c);

	bool looping = true;
	while (looping)
	{
		//Retrieves direction to new 'b', currentDir is always the last dir minus 1 (eg dir 7 means currentDir 6, 0 means 7 etc)
		int new_direction = getDirectionClosest(image, b, current_direction);

		//Retrieves coords of new 'b'
		next_point = getDirectionPoint(b, new_direction);

		//Checks if new point isnt already added and that dir has indeed found a 1 (-1 means an incorrect direction or none was found)
		// a -1 or already added point prompts the statement below to cycle to the next direction and check again
		if (!containsPoint(contour, next_point) && new_direction != -1)
		{
			//Stop while loop if new b is found
			looping = false;
			//Make sure currentDir doesnt go below 0
			if (new_direction > 0)
				current_direction = new_direction - 1;
			else if (new_direction == 0)
				current_direction = 7;
		}
		else
		{
			if (current_direction < 7)
				current_direction++;
			else
				current_direction = 0;
		}
		//Check if origin is reached, if so abort
		if (next_point.x == origin->y && next_point.y == origin->x)
		{
			return;
		}
	}

	//TODO: remove this check, should not be needed as this is already being checked in the aforementioned while loop
	if (!containsPoint(contour, next_point))
	{
		//Calculate the coords of the last 0 preceding nextPoint
		Point newC = getDirectionPoint(b, current_direction);
		cout << "pointC: " << newC << endl;
		cout << "pointB: " << next_point << endl;
		contour.push_back(next_point);
		findNextContourPixel(image, next_point, newC, contour, origin);
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
 * Turns clockwise around a point in a binary image_binary to find a 1 based on a starting direction
 * returns -1 if no 1 was found in range
 */
int getDirectionClosest(Mat binary_image, Point point, int staring_direction)
{
	for (int i = staring_direction; i < 8; i++)
	{
		Point newPoint = getDirectionPoint(point, i);
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
 *    0 p1 4
 *    7 6 5
 * returns -1 if points are incorrect
 */
int getDirectionNumber(Point p1, Point p2)
{
	if (p2.x == p1.x - 1 && p2.y == p1.y)
		return 0;
	if (p2.x == p1.x - 1 && p2.y == p1.y - 1)
		return 1;
	if (p2.x == p1.x && p2.y == p1.y - 1)
		return 2;
	if (p2.x == p1.x + 1 && p2.y == p1.y - 1)
		return 3;
	if (p2.x == p1.x + 1 && p2.y == p1.y)
		return 4;
	if (p2.x == p1.x + 1 && p2.y == p1.y + 1)
		return 5;
	if (p2.x == p1.x && p2.y == p1.y + 1)
		return 6;
	if (p2.x == p1.x - 1 && p2.y == p1.y + 1)
		return 7;

	return -1;
}

/*
 * returns a new point based on a current point and a direction
 *    1 2 3
 *    0 p1 4
 *    7 6 5
 * returns Point(-1,-1) if a incorrect direction is supplied
 */
Point getDirectionPoint(Point p, int direction_number)
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
			chainCode.push_back(getDirectionNumber(contourVec[i], contourVec[i + 1]));
		else
			chainCode.push_back(getDirectionNumber(contourVec[i], contourVec[0]));
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

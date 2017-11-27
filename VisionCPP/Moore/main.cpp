#include "stdafx.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <stdio.h>
#include <cmath>

using namespace std;
using namespace cv;

int allContoursRecursive(Mat image_binary, vector<vector<Point>>& contours);
int getDirectionNumber(Point p1, Point p2);
int getDirectionClosest(Mat image_binary, Point p, int starting_direction = 0);
Point getDirectionPoint(Point p, int direction_number);
void nextPoint(Mat image_binary, vector<Point>& contour, Point2d current_pixel, int current_direction);
void getContour(Mat image_binary, vector<Point>& contour, Point2d first_pixel);
double bendingEnergy(vector<Point>& contourVec);

int allContours(Mat image_binary, vector<vector<Point>>& contours)
{
	Mat image_labled;
	vector<Point2d *> firstPixels;
	vector<Point2d *> centers;
	vector<int> areas;
	labelBLOBsInfo(image_binary, image_labled, firstPixels, centers, areas);

	for (Point2d* firstPixel : firstPixels)
	{
		cout << "Getting contour for figure with first pixel ["
			<< firstPixel->x << "," << firstPixel->y << "] = "
			<< getEntryImage(image_binary, firstPixel->x, firstPixel->y)
			<< endl;

		// Start creating vector which will contain the contour
		vector<Point> contour;
		// add first pixel 
		contour.push_back(Point(firstPixel->y, firstPixel->x));
		// find next pixel
		int current_direction = getDirectionClosest(image_binary, Point(firstPixel->y, firstPixel->x));
		Point next_point = getDirectionPoint(Point(firstPixel->y, firstPixel->x), current_direction);
		contour.push_back(next_point);
		int next_direction = getDirectionClosest(image_binary, next_point, current_direction - 2);
		next_point = getDirectionPoint(next_point, next_direction);
		do
		{
			contour.push_back(next_point);
			next_direction = getDirectionClosest(image_binary, next_point, next_direction - 2);
			next_point = getDirectionPoint(next_point, next_direction);
		}
		while (!(firstPixel->y == next_point.x && firstPixel->x == next_point.y));

		contours.push_back(contour);
		cout << "contour done" << endl;
	}
	return -1;
}

int main()
{
	Mat image_original = imread("./../Images/bladeren.jpg", CV_LOAD_IMAGE_COLOR);
	imshow("Original", image_original);
	waitKey(0);

	Mat image_gray;
	cvtColor(image_original, image_gray, CV_BGR2GRAY);
	imshow("Gray", image_gray);
	waitKey(0);

	Mat image_blur;
	GaussianBlur(image_gray, image_blur, Size(3, 3), 0, 0);
	imshow("Blur", image_blur);
	waitKey(0);

	Mat image_dilate;
	dilate(image_blur, image_dilate, getStructuringElement(MORPH_RECT, Size(5, 5), Point(1, 1)));
	imshow("dilate", image_dilate);

	Mat image_erode;
	erode(image_dilate, image_erode, getStructuringElement(MORPH_RECT, Size(5, 5), Point(1, 1)));
	imshow("erode", image_erode);
	waitKey(0);

	Mat image_binary;
	threshold(image_erode, image_binary, 200, 1, CV_THRESH_BINARY_INV);
	imshow("Binary", image_binary);
	waitKey(0);

	Mat image_binary16S;
	image_binary.convertTo(image_binary16S, CV_16S);
	show16SImageStretch(image_binary16S, "Binary 16S");
	waitKey(0);

	vector<vector<Point>>* contours = new vector<vector<Point>>();
	allContours(image_binary16S, *contours);
	for (vector<Point> contour : *contours)
	{
		cout << bendingEnergy(contour) << endl;
	}
	waitKey(0);

	Mat contourMat = image_binary.clone();
	contourMat = 0;

	for (vector<Point> src : *contours)
	{
		for (Point e : src)
			contourMat.at<uchar>(Point(e.x, e.y)) = 255;
	}
	imshow("contours", contourMat);
	waitKey(0);
}

/*
* finds all contours in an image_binary
*/
int allContoursRecursive(Mat image_binary, vector<vector<Point>>& contours)
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

		// Start creating vector which will contain the contour
		vector<Point> contour;
		getContour(image_binary, contour, Point(firstPixel->y, firstPixel->x));
		contours.push_back(contour);
		cout << "contour done" << endl;
	}
	return 0;
}

/*
 * Finds all contours in a image
 *  */
void getContour(Mat image_binary, vector<Point>& contour, Point2d first_pixel)
{
	contour.push_back(first_pixel);
	int current_direction = getDirectionClosest(image_binary, first_pixel);
	Point next_point = getDirectionPoint(first_pixel, current_direction);
	nextPoint(image_binary, contour, next_point, current_direction);
}

void nextPoint(Mat image_binary, vector<Point>& contour, Point2d current_pixel, int current_direction)
{
	contour.push_back(current_pixel);
	int next_direction = getDirectionClosest(image_binary, current_pixel, current_direction - 2);
	Point next_point = getDirectionPoint(current_pixel, next_direction);
	if (!(contour[0].x == next_point.x && contour[0].y == next_point.y))
	{
		nextPoint(image_binary, contour, next_point, next_direction);
	}
}

/*
 * Turns clockwise around a point in a binary image_binary to find a 1 based on a starting direction
 *    1 2 3
 *    0 p 4
 *    7 6 5
 * returns -1 if no 1 was found in range
 */
int getDirectionClosest(Mat binary_image, Point point, int staring_direction)
{
	int current_direction = staring_direction;
	if (current_direction == -1)
		current_direction = 7;
	if (current_direction == -2)
		current_direction = 6;
	for (int i = 0; i < 8; i++)
	{
		if (current_direction < 8)
		{
			Point newPoint = getDirectionPoint(point, current_direction);
			if (getEntryImage(binary_image, newPoint.y, newPoint.x) == 1)
			{
				return current_direction;
			}
			current_direction++;
		}
		else
		{
			current_direction = 0;
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
 *    0 p 4
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

/*
 * calculates the bending energy
 */
double bendingEnergy(vector<Point>& contour)
{
	vector<int> chainCode;
	for (int i = 0; i < contour.size(); i++)
	{
		if (i + 1 < contour.size())
			chainCode.push_back(getDirectionNumber(contour[i], contour[i + 1]));
		else
			chainCode.push_back(getDirectionNumber(contour[i], contour[0]));
	}
	double bendingSum = 0;
	for (int i = 1; i < chainCode.size(); i++)
	{
		bendingSum += abs(chainCode[i] - chainCode[i - 1]);
	}
	return bendingSum;
}

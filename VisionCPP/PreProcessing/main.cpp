#include "stdafx.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <stdio.h>
#include <cmath>
#include <algorithm>

using namespace std;
using namespace cv;



int main()
{

	Mat image_original = imread("./../Images/basisfiguren.jpg", CV_LOAD_IMAGE_COLOR);

	imshow("Original", image_original);

	Mat image_gray;
	cvtColor(image_original, image_gray, CV_BGR2GRAY);
	imshow("Gray", image_gray);

	Mat image_binary;
	threshold(image_gray, image_binary, 200, 1, CV_THRESH_BINARY_INV);
	imshow("Binary", image_binary);

	Mat image_binary16S;
	image_binary.convertTo(image_binary16S, CV_16S);
	show16SImageStretch(image_binary16S, "Binary 16S");

	vector<vector<Point>>* contours = new vector<vector<Point>>();
	allContours(image_binary16S, *contours);

	Mat contoursMat;
	drawPoints(image_binary, contoursMat, contours);
	imshow("contours", contoursMat);

	vector<vector<Point>>* bounding_boxes = new vector<vector<Point>>();
	allBoundingBoxes(*contours, *bounding_boxes);

	Mat image_bounding_boxes;
	drawBoundingBox(image_original, image_bounding_boxes, bounding_boxes);
	imshow("boundingBoxes", image_bounding_boxes);

	compartMentalise(image_original, "figuur");
 	

	vector<vector<Point>>* regionPixels = new vector<vector<Point>>();
	for (vector<Point> contour : *contours)
	{
		vector<Point> regionPixel;
		enclosedPixels(contour, regionPixel,image_original);
		regionPixels->push_back(regionPixel);
	}

	Mat region;
	drawPoints(image_binary, region, regionPixels);
	imshow("region", region);
	waitKey(30);

	compartMentaliseFloodFill(image_original, "figuurFF");
	waitKey(0);
}

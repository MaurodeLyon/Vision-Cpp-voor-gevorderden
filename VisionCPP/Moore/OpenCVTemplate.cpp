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
bool containsPoint(vector<Point>, Point);


int allContours(Mat binaryImage, vector< vector<Point> > & contourVecVec);


int main() {

	Mat frame = imread("C:/Users/Arthur/Documents/SourceTree/Vision-Cpp-voor-gevorderden/VisionCPP/Moore/basisfiguren.jpg", CV_LOAD_IMAGE_COLOR);

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

	for (vector<Point> vec: *contourVecVec )
	{
		for(Point e :  vec)
		circle(gray_image, e, 3, Scalar(128, 128, 128));
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
	binaryImage = (Mat_<double>(6, 6) << 0, 0, 0, 0, 0, 0,
		0,1,1,1,0,0,
		0,1,0,0,1,0,
		0,0,1,0,1,0,
		0,1,0,0,1,0,
		0,1,1,1,0,0);

	vector<Point2d *> firstpixelVec2;
	vector<Point2d *> posVec2;
	vector<int> areaVec2;
	labelBLOBsInfo(binaryImage, labeledImage, firstpixelVec2, posVec2, areaVec2);


	Point2d *firstB0;
	//Point2d firstC0;


	for (Point2d *ptr : firstpixelVec2)
	{
		vector<Point> obj;
		currentDir = 0;
		firstB0 = ptr;
		//Check if bgX is not 0?
		//c0 = Point2d(b0->x - 1, b0->y);
		//firstC0 = retrieveNextC(labeledImage, *firstB0);
		bool looped = false;

		Point2d b = Point2d(firstB0->x,firstB0->y);
		obj.push_back(b);
		while (!looped)
		{
			b = retrieveNextB(binaryImage, b);

			int val = getEntryImage(binaryImage,b.x,b.y);

			if (containsPoint(obj, b))
			{
				looped = true;
			}
			else
			{
				obj.push_back(b);
			}

			
		}
		contourVecVec.push_back(obj);

		


	}

	imshow("labeled", labeledImage);

	return 1;
	
	
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

Point2d retrieveNextB(Mat img, Point2d b)
{
	Point2d nextB(0,0);
	
	uchar val;
	Scalar colour;
	switch (currentDir)
	{
	case posC::E:
		val = getEntryImage(img, b.x - 1, b.y);
		if (val==1)
		{
			nextB = Point2d(b.x-1, b.y );
			break;
		}
		else
		{
			currentDir++;
		}
	case posC::NE:
		val = getEntryImage(img, b.x - 1, b.y-1);
		if (val == 1)
		{
			nextB = Point2d(b.x-1, b.y - 1);
			break;
		}
		else
		{
			currentDir++;
		}

	case posC::N:
		val = getEntryImage(img, b.x, b.y-1);
		if (val == 1)
		{
			nextB = Point2d(b.x, b.y - 1);
			break;
		}
		else
		{
			currentDir++;
		}

	case posC::NW:
		val = getEntryImage(img, b.x +1, b.y-1);
		if (val==1)
		{
			nextB = Point2d(b.x+1, b.y - 1);
			break;
		}
		else
		{
			currentDir++;
		}
	case posC::W:
		val = getEntryImage(img, b.x + 1, b.y);
		if (val==1)
		{
			nextB = Point2d(b.x+1, b.y);
			break;
		}
		else
		{
			currentDir++;
		}
	case posC::SW:
		val = getEntryImage(img, b.x + 1, b.y-1);
		if (val==1)
		{
			nextB = Point2d(b.x+1, b.y - 1);
			break;
		}
		else
		{
			currentDir++;
		}
	case posC::S:
		val = getEntryImage(img, b.x, b.y+1);
		if (val==1)
		{
			nextB = Point2d(b.x, b.y + 1);
			break;
		}
		else
		{

			currentDir++;
		}
	case posC::SE:
		val = getEntryImage(img, b.x, b.y-1);
		if (val==1)
		{
			nextB = Point2d(b.x, b.y - 1);
			break;
		}
		else
		{
			return NULL;
		}
	

	}

	return nextB;

}




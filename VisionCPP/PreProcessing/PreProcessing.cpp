#include "stdafx.h"
#include "PreProcessing.h"


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
double bendingEnergy(Mat image_binary, vector<Point>& contour)
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
		if (i + 1 < contour.size())
			bendingSum += abs(chainCode[i] - chainCode[i - 1]);
		else
			bendingSum += abs(chainCode[i] - chainCode[0]);
	}
	return bendingSum/chainCode.size();
}

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

		if (firstPixel->x != 0 && firstPixel->y != 0)
		{
			// Start creating vector which will contain the contour
			vector<Point> contour;
			// add first pixel 
			contour.push_back(Point(firstPixel->y, firstPixel->x));
			// find next pixel
			int current_direction = getDirectionClosest(image_binary, Point(firstPixel->y, firstPixel->x));
			Point next_point = getDirectionPoint(Point(firstPixel->y, firstPixel->x), current_direction);
			if (!(next_point.y == -1 && next_point.x == -1))
			{
				contour.push_back(next_point);
				int next_direction = getDirectionClosest(image_binary, next_point, current_direction - 2);
				next_point = getDirectionPoint(next_point, next_direction);

				do
				{
					contour.push_back(next_point);
					next_direction = getDirectionClosest(image_binary, next_point, next_direction - 2);
					next_point = getDirectionPoint(next_point, next_direction);
				} while (!(firstPixel->y == next_point.x && firstPixel->x == next_point.y));
			}
			contours.push_back(contour);
			cout << "contour done" << endl;
		}
		else
		{
			cout << "failure first point, ignoring for to conform with testing parameters" << endl;
		}

		
	}
	return -1;
}

int getBoundingBox(const vector<Point>& contour, vector<Point>& bounding_box)
{
	int xmin = 0, ymin = 0, xmax = -1, ymax = -1;
	Point left_border, top_border, right_border, bottom_border;

	left_border = top_border = right_border = bottom_border = contour[0];
	xmin = xmax = contour[0].x;
	ymin = ymax = contour[0].y;

	for (int i = 1; i < contour.size(); i++)
	{
		Point point = contour[i];

		if (xmin > point.x)
		{
			xmin = point.x;
			left_border = point;
		}

		if (xmax < point.x)
		{
			xmax = point.x;
			right_border = point;
		}

		if (ymin > point.y)
		{
			ymin = point.y;
			top_border = point;
		}

		if (ymax < point.y)
		{
			ymax = point.y;
			bottom_border = point;
		}
	}

	bounding_box.push_back(left_border);
	bounding_box.push_back(right_border);
	bounding_box.push_back(top_border);
	bounding_box.push_back(bottom_border);




	return 1;

}


// func: delivers bounding Boxes of contours
// pre: contours contains the contours for which bounding boxes have to be delivered
// post: boundingBoxes contains all bounding boxes. The index corresponds to the index of contours.
// I.e. boundingBoxes[i] belongs to contours[i]
int allBoundingBoxes(const vector<vector<Point>>& contours, vector<vector<Point>>& bounding_boxes)
{
	for (vector<Point> contour : contours)
	{
		vector<Point> *bounding_box = new vector<Point>();
		getBoundingBox(contour, *bounding_box);
		bounding_boxes.push_back(*bounding_box);
	}
	return 1;
}

//Requires a uneditied image and splits it into one image per object each
int compartMentalise(Mat image_original, string name)
{
	Mat image_gray;
	cvtColor(image_original, image_gray, CV_BGR2GRAY);

	Mat image_binary;
	threshold(image_gray, image_binary, 200, 1, CV_THRESH_BINARY_INV);

	Mat image_binary16S;
	image_binary.convertTo(image_binary16S, CV_16S);

	vector<vector<Point>>* contours = new vector<vector<Point>>();
	allContours(image_binary16S, *contours);

	Mat boundingBoxes = image_original.clone();

	vector<vector<Point>>* bounding_boxes = new vector<vector<Point>>();

	allBoundingBoxes(*contours, *bounding_boxes);

	for (int i = 0; i < bounding_boxes->size(); i++)
	{
		vector<Point> bounding_box = bounding_boxes->at(i);
		Rect rect = Rect(bounding_box[0].x - 1, bounding_box[2].y - 1, bounding_box[1].x - bounding_box[0].x + 3,
			bounding_box[3].y - bounding_box[2].y + 3);
		Mat image_roi = image_original(rect);

		imwrite("./../Images/trainingset/" + name + "_" + to_string(i) + ".jpg", image_roi);
	}
	return 1;
}

int compartMentaliseFloodFill(Mat image_original, string name)
{
	Mat image_gray;
	cvtColor(image_original, image_gray, CV_BGR2GRAY);

	Mat image_binary;
	threshold(image_gray, image_binary, 200, 1, CV_THRESH_BINARY_INV);

	Mat image_binary16S;
	image_binary.convertTo(image_binary16S, CV_16S);

	vector<vector<Point>>* contours = new vector<vector<Point>>();
	allContours(image_binary16S, *contours);

	vector<vector<Point>>* bounding_boxes = new vector<vector<Point>>();

	allBoundingBoxes(*contours, *bounding_boxes);
	vector<vector<Point>>* regionPixels = new vector<vector<Point>>();
	for (vector<Point> contour : *contours)
	{
		vector<Point> regionPixel;
		enclosedPixels(contour, regionPixel, image_original);
		regionPixels->push_back(regionPixel);
	}
	Mat region;
	drawPoints(image_binary, region, regionPixels);


	for (int i = 0; i < bounding_boxes->size(); i++)
	{
		vector<Point> bounding_box = bounding_boxes->at(i);
		Rect rect = Rect(bounding_box[0].x, bounding_box[2].y, bounding_box[1].x - bounding_box[0].x + 1,
			bounding_box[3].y - bounding_box[2].y + 1);
		Mat image_roi = region(rect);
		Mat canvas = image_roi;

		///TODO: Make sure the index of bounding_boxes and regionPixels are synched (should already be,
		///but not guarenteed)


		imwrite("./../Images/trainingset/" + name + "_" + to_string(i) + "FF" + ".jpg", canvas);
		imshow(name + "_" + to_string(i), canvas);
	}
	return 1;

}

/*
*  outputs an image which displays the bounding box
*/
void drawBoundingBox(Mat image_original, Mat& image_bounding_boxes, vector<vector<Point>>* bounding_boxes)
{
	{
		image_bounding_boxes = image_original.clone();
		for (vector<Point> bounding_box : *bounding_boxes)
		{
			for (Point point : bounding_box)
			{
				ellipse(image_bounding_boxes, point, Size(3, 3), 0, 0, 360, Scalar(255, 0, 255), 2, 8, 0);
			}
			Rect rect = Rect(bounding_box[0].x, bounding_box[2].y, bounding_box[1].x - bounding_box[0].x + 1,
				bounding_box[3].y - bounding_box[2].y + 1);
			rectangle(image_bounding_boxes, rect, Scalar(rand() & 255, rand() & 255, rand() & 255), 1, 8);
		}
	}
}

/*
* Start function for recursion
*/
int enclosedPixels(const vector<Point>& contour, vector<Point>& region, Mat debugImg)
{

	Point startPixel;
	bool found = false;

	while (!found)
	{
		vector<Point> *outerPoints = new vector<Point>();
		getBoundingBox(contour, *outerPoints);

		startPixel.x = rand() % (outerPoints->at(1).x - outerPoints->at(0).x) + outerPoints->at(0).x;
		startPixel.y = rand() % (outerPoints->at(3).y - outerPoints->at(2).y) + outerPoints->at(2).y;

		if (isPointInRange(contour, startPixel))
			found = true;


	}

	region.push_back(startPixel);

	vector<Point> edge;
	edge.push_back(startPixel);
	vector<Point> newEdge;
	do
	{
		newEdge.clear();
		for (Point p : edge)
		{
			if (!containsPoint(contour, Point(p.x - 1, p.y)) && !containsPoint(region, Point(p.x - 1, p.y)))
				if (!containsPoint(newEdge, Point(p.x - 1, p.y)))
					newEdge.push_back(Point(p.x - 1, p.y));


			if (!containsPoint(contour, Point(p.x, p.y - 1)) && !containsPoint(region, Point(p.x, p.y - 1)))
				if (!containsPoint(newEdge, Point(p.x, p.y - 1)))
					newEdge.push_back(Point(p.x, p.y - 1));

			if (!containsPoint(contour, Point(p.x + 1, p.y)) && !containsPoint(region, Point(p.x + 1, p.y)))
				if (!containsPoint(newEdge, Point(p.x + 1, p.y)))
					newEdge.push_back(Point(p.x + 1, p.y));

			if (!containsPoint(contour, Point(p.x, p.y + 1)) && !containsPoint(region, Point(p.x, p.y + 1)))
				if (!containsPoint(newEdge, Point(p.x, p.y + 1)))
					newEdge.push_back(Point(p.x, p.y + 1));
		}
		//region.insert(region.end(), newEdge.begin(), newEdge.end());
		for (Point p : newEdge)
			region.push_back(p);
		edge = newEdge;
		Mat drawEdges;
		//vector<Point> *regionPtr = &region;
		drawPoints(debugImg, drawEdges, &region);
		imshow("floodfilldebug", drawEdges);
		waitKey(30);

	} while (newEdge.size() > 0);
	return 0;
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

/*
* function returns image with the contours if the binary image
*/
void drawPoints(Mat image_binary, Mat& image_contour, vector<vector<Point>>* contours)
{
	image_contour = image_binary.clone();
	image_contour = 0;
	for (vector<Point> vec : *contours)
	{
		for (Point e : vec)
			image_contour.at<uchar>(Point(e.x, e.y)) = 255;
	}
}

void drawPoints(Mat image_binary, Mat& image_contour, vector<Point>* contour)
{
	image_contour = image_binary.clone();
	image_contour = 0;
	for (Point e : *contour)
		image_contour.at<uchar>(Point(e.x, e.y)) = 255;

}

bool isPointInRange(const std::vector<cv::Point>& contour, cv::Point p)
{
	bool correct = false;
	if (containsPoint(contour, p))
		return false;

	for (int i = 0; i < contour.size(); ++i) {

		int j = (i + 1) % contour.size();

		if ((contour[i].y <= p.y) && (contour[j].y > p.y) || (contour[j].y <= p.y) && (contour[i].y > p.y)) {

			double crossedVal = (contour[j].x - contour[i].x) * (p.y - contour[i].y) / (contour[j].y - contour[i].y) + contour[i].x;

			if (crossedVal < p.x)
				correct = !correct;
		}
	}

	return correct;
}

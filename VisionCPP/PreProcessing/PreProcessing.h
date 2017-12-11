#pragma once

	int getDirectionNumber(Point p1, Point p2);
	int getDirectionClosest(Mat image_binary, Point p, int starting_direction = 0);
	Point getDirectionPoint(Point p, int direction_number);

	int allContoursRecursive(Mat image_binary, vector<vector<Point>>& contours);
	void nextPoint(Mat image_binary, vector<Point>& contour, Point2d current_pixel, int current_direction);
	void getContour(Mat image_binary, vector<Point>& contour, Point2d first_pixel);
	void drawPoints(Mat image_binary, Mat& image_contour, vector<vector<Point>>* contours);
	void drawPoints(Mat image_binary, Mat& image_contour, vector<Point>* contours);
	double bendingEnergy(Mat binaryImage, vector<Point>& contourVec);
	int allContours(Mat image_binary, vector<vector<Point>>& contours);


	// Fase 2
	int allBoundingBoxes(const vector<vector<Point>>& contours, vector<vector<Point>>& bounding_boxes);
	int getBoundingBox(const vector<Point>& contour, vector<Point>& bounding_boxes);
	int compartMentalise(Mat image_original, string name);
	int compartMentaliseFloodFill(Mat image_original, string name);
	void drawBoundingBox(Mat image_original, Mat& image_bounding_boxes, vector<vector<Point>>* bounding_boxes);

	bool containsPoint(vector<Point> points, Point p);
	int enclosedPixels(const vector<Point>& contourVec, vector<Point>& regionPixels, Mat debugImg);
	bool isPointInRange(const std::vector<cv::Point>& contour, cv::Point p);
	



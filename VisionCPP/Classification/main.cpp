#include "stdafx.h"
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;


int main(int argc, char** argv)
{
	cout << "---- Mauro & Arthur object classification application ----" << endl;

	// Train neural network with training data

	///Create test data
	//cout << "Press enter to check the object" << endl;
	//string input;
	//getline(cin, input);

	//VideoCapture cap(1);
	////waitKey(500);
	//if (!cap.isOpened())
	//	cout << "Cannot open the video cam" << endl;

	//double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	//double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	//cout << "Frame size : " << dWidth << " x " << dHeight << endl;

	//Mat frame;
	//int i = 0;
	//string filename = "vis";
	//while (true)
	//{
	//	cap.read(frame);
	//	imshow("test", frame);
	//	if (waitKey(30) == 27)
	//	{
	//		std::cout << "save object" << std::endl;
	//		
	//		//getline(cin,filename);
	//		imwrite("./../Images/trainingset/" + filename + "_" + to_string(i) + ".jpg", frame);
	//		i++;
	//	}
	//}

	Mat src = imread("./../Images/trainingset/skyrim_0.jpg", CV_LOAD_IMAGE_COLOR);
	Mat gray;
	cvtColor(src, gray, CV_BGR2GRAY);
	Mat binaryImg; Mat binaryImgINV;
	threshold(gray, binaryImg, 200, 255, THRESH_BINARY);
	threshold(gray, binaryImgINV, 200, 1, THRESH_BINARY_INV);
	Mat binaryImg16S;
	binaryImgINV.convertTo(binaryImg16S, CV_16S);
	imshow("src", src);
	imshow("gray", gray);
	imshow("binaryImg", binaryImg);
	waitKey(0);


	//Nr of holes

	vector< vector <Point> > contoursOpenCV; // Vector for storing contour
	vector< Vec4i > hierarchy;
	//Mat dst(src.rows, src.cols, CV_8UC1, Scalar::all(0)); //create destination image
	Mat dst = src.clone();
	
	//dst = 0;
	int count = 0;
	//allContours(binaryImg16S, contours);
	findContours(binaryImg, contoursOpenCV, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE); // Find the contours in the image

	for (int i = 0; i< contoursOpenCV.size(); i = hierarchy[i][0]) // iterate through each contour.
	{
		Rect r = boundingRect(contoursOpenCV[i]);
		if (hierarchy[i][2]<0) {
			rectangle(dst, Point(r.x, r.y), Point(r.x + r.width, r.y + r.height), Scalar(255, 0, 0), 3, 8, 0);
			count++;
		}
	}
	cout << "Number of contour = " << count << endl;
	
	imshow("nrOfHoles", dst);
	waitKey(0);

	//Bending Energy
	vector<vector<Point>>* contours = new vector<vector<Point>>();
	allContours(binaryImg16S, *contours);
	for (vector<Point> contour : *contours) {
		cout << "bendingEnergy " << bendingEnergy(binaryImg16S, contour);
	}
	waitKey(0);
	



	// Get the camera image
	// Detect object
	// Extract object 
	//     1. contour
	//     2. flood fill
	//     3. output image
	// Feed the output image to the neural network
	// Display the results
}

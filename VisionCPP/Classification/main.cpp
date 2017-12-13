#include "stdafx.h"
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	cout << "---- Mauro & Arthur object classification application ----" << endl;

	// Train neural network with training data

	cout << "Press enter to check the object" << endl;
	string input;
	getline(cin, input);

	VideoCapture cap(1);
	//waitKey(500);
	if (!cap.isOpened())
		cout << "Cannot open the video cam" << endl;

	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	cout << "Frame size : " << dWidth << " x " << dHeight << endl;

	Mat frame;
	int i = 0;
	string filename = "vis";
	while (true)
	{
		cap.read(frame);
		imshow("test", frame);
		if (waitKey(30) == 27)
		{
			std::cout << "save object" << std::endl;
			
			//getline(cin,filename);
			imwrite("./../Images/trainingset/" + filename + "_" + to_string(i) + ".jpg", frame);
			i++;
		}
	}
		

	// Get the camera image
	// Detect object
	// Extract object 
	//     1. contour
	//     2. flood fill
	//     3. output image
	// Feed the output image to the neural network
	// Display the results
}

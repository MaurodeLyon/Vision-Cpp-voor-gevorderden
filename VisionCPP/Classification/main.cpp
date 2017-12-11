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

	// Get the camera image
	// Detect object
	// Extract object 
	//     1. contour
	//     2. flood fill
	//     3. output image
	// Feed the output image to the neural network
	// Display the results
}

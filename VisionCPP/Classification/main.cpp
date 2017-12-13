#include "stdafx.h"

using namespace cv;
using namespace std;

void loadSquareImageTrainingSet(Mat& ITset, Mat& OTset)
{
	Mat image_gray;
	cvtColor(imread("./../Images/trainingset/test_0.bmp", CV_LOAD_IMAGE_COLOR), image_gray, CV_BGR2GRAY);
	imshow("Gray", image_gray);
	Mat image_binary;
	threshold(image_gray, image_binary, 200, 1, CV_THRESH_BINARY_INV);
	image_binary.convertTo(image_binary, CV_16S);
	printMatrix(image_binary);
	
	Mat blob;
	cout << labelBLOBs(image_binary, blob) << endl;

	ITset = (Mat_<double>(2, 1) <<
		0 ,
		1);

	OTset = (Mat_<double>(2, 1) <<
		1 ,
		0);
}

const double MAX_OUTPUT_ERROR = 1E-10;
const int MAXRUNS = 10000;

int main(int argc, char** argv)
{
	Mat ITset, OTset;
	loadSquareImageTrainingSet(ITset, OTset);
	
//	Mat V0, W0, dW0, dV0;
//
//	int hiddenNeurons = 2;
//
//	cout << " ===> BPN format: " << endl <<
//		"BPN Inputlayer  = " << ITset.cols << "  neurons" << endl <<
//		"BPN Outputlayer = " << OTset.cols << "  neurons" << endl << endl;
//	cout << "Please choose a number of hidden neurons: ";
//	cin >> hiddenNeurons;
//
//	cout << "Initialize BPN ..." << endl;
//	initializeBPN(ITset.cols, hiddenNeurons, OTset.cols, V0, dV0, W0, dW0);
//	cout << "Press ENTER => ";
//	string dummy;
//	getline(cin, dummy);
//	getline(cin, dummy);
//
//	Mat IT, OT, OH, OO;
//	double outputError0, outputError1, sumSqrDiffError = MAX_OUTPUT_ERROR + 1;
//	Mat V1, W1;
//	int runs = 0;
//	while ((sumSqrDiffError > MAX_OUTPUT_ERROR) && (runs < MAXRUNS))
//	{
//		sumSqrDiffError = 0;
//
//		for (int inputSetRowNr = 0; inputSetRowNr < ITset.rows; inputSetRowNr++)
//		{
//			IT = transpose(getRow(ITset, inputSetRowNr));
//			OT = transpose(getRow(OTset, inputSetRowNr));
//			calculateOutputHiddenLayer(IT, V0, OH);
//			calculateOutputBPN(OH, W0, OO);
//			adaptVW(OT, OO, OH, IT, W0, dW0, V0, dV0, W1, V1);
//			calculateOutputBPNError(OO, OT, outputError0);
//			calculateOutputBPNError(BPN(IT, V1, W1), OT, outputError1);
//			sumSqrDiffError += (outputError1 - outputError0) * (outputError1 - outputError0);
//			V0 = V1;
//			W0 = W1;
//		}
//		cout << "sumSqrDiffError = " << sumSqrDiffError << endl;
//		runs++;
//	}
//
//	cout << "BPN Training is ready!" << endl << endl;
//	cout << "Runs = " << runs << endl << endl;
//
//	Mat inputVectorTrainingSet, outputVectorTrainingSet, outputVectorBPN;
//	cout << setw(1) <<
//		" Training Input" << setw(12) <<
//		"| Expected Output " << setw(1) <<
//		"| Output BPN " << setw(6) <<
//		"|" << endl << endl;
//	for (int row = 0; row < ITset.rows; row++)
//	{
//		inputVectorTrainingSet = transpose(getRow(ITset, row));
//		for (int r = 0; r < inputVectorTrainingSet.rows; r++)
//			cout << setw(8) << getEntry(inputVectorTrainingSet, r, 0);
//		cout << setw(2) << "|";
//
//		outputVectorTrainingSet = transpose(getRow(OTset, row));
//
//		for (int r = 0; r < outputVectorTrainingSet.rows; r++)
//			cout << setw(8) << round(getEntry(outputVectorTrainingSet, r, 0));
//		cout << setw(2) << "|";
//		outputVectorBPN = BPN(inputVectorTrainingSet, V0, W0);
//		for (int r = 0; r < outputVectorBPN.rows; r++)
//			cout << setw(8) << getEntry(outputVectorBPN, r, 0);
//		cout << setw(2) << "|";
//		cout << endl;
//	}
//	Mat test = (Mat_<double>(2, 1) << 1 , 0);
//	Mat result = BPN(test, V0, W0);
//	cout << "results : " << endl;
//	for (int row = 0; row < result.rows; row++)
//	{
//		for (int col = 0; col < result.cols; col++)
//		{
//			cout << getEntry(result, row, col);
//		}
//		cout << endl;
//	}
//	cout << endl << endl << "Press ENTER for exit";
//	getline(cin, dummy);
//	getline(cin, dummy);
	
	/*cout << "---- Mauro & Arthur object classification application ----" << endl;

	// Train neural network with training data

	cout << "Press enter to check the object" << endl;
	string input;
	getline(cin, input);

	// Get the camera image
	Mat image_original = imread("./../Images/square.bmp", CV_LOAD_IMAGE_COLOR);

	// Detect object
	Mat image_gray;
	cvtColor(image_original, image_gray, CV_BGR2GRAY);
	Mat image_binary;
	threshold(image_gray, image_binary, 200, 1, CV_THRESH_BINARY_INV);
	Mat image_binary16S;
	image_binary.convertTo(image_binary16S, CV_16S);

	// Extract object 
	//     1. contour
	//     2. flood fill
	//     3. output image
	compartMentaliseFloodFill(image_original, "figuurFF");
	// feature detection
	// Feed the output image to the neural network
	// Display the results

	imshow("Original", image_original);
	show16SImageStretch(image_binary16S, "Binary 16S");*/
}

#include "stdafx.h"
#include "PreProcessing.h"

using namespace cv;
using namespace std;

int numberOfHolesID(Mat image_binary);
double bendingEnergyID(Mat image_binary_16s);

void loadSquareImageTrainingSet(Mat& ITset, Mat& OTset)
{
	// load image
	Mat image = imread("./../Images/trainingSquare.png", CV_LOAD_IMAGE_COLOR);
	cvtColor(image, image, CV_BGR2GRAY);
	threshold(image, image, 200, 1, CV_THRESH_BINARY_INV);
	image.convertTo(image, CV_16S);
	show16SImageStretch(image);

	// collect data
	Mat blobs;
	int numberOfHoles = numberOfHolesID(image);
	vector<vector<Point>> contours;
	allContours(image, contours);
	double energy = bendingEnergyID(image);
	// create input set

	// create desired output
	waitKey(0);
	//	Mat image_gray;
	//	cvtColor(imread("./../Images/trainingSquare.png", CV_LOAD_IMAGE_COLOR), image_gray, CV_BGR2GRAY);
	//	imshow("Gray", image_gray);
	//	Mat image_binary;
	//	threshold(image_gray, image_binary, 200, 1, CV_THRESH_BINARY_INV);
	//	image_binary.convertTo(image_binary, CV_16S);
	//	//printMatrix(image_binary);
	//	
	//	Mat blob;
	//	cout << labelBLOBs(image_binary, blob) << endl;
	//
	//	ITset = (Mat_<double>(2, 1) <<
	//		0 ,
	//		1);
	//
	//	OTset = (Mat_<double>(2, 1) <<
	//		1 ,
	//		0);
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
	*/
}

int numberOfHolesID(Mat image_binary)
{
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	int count = 0;
	findContours(image_binary, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < contours.size(); i = hierarchy[i][0])
	{
		if (hierarchy[i][2] < 0)
		{
			count++;
		}
	}
	return count;
}

double bendingEnergyID(Mat image_binary_16s)
{
	//Bending Energy
	vector<vector<Point>>* contours = new vector<vector<Point>>();
	allContours(image_binary_16s, *contours);
	double bendingEnergyID = 0;
	for (vector<Point> contour : *contours)
	{
		bendingEnergyID = bendingEnergy(image_binary_16s, contour);
	}
	return bendingEnergyID;
}

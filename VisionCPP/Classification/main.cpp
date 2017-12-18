#include "stdafx.h"
#include "PreProcessing.h"

using namespace cv;
using namespace std;

int numberOfHolesID(Mat image_binary);
double bendingEnergyID(Mat image_binary_16s);

Mat_<double> Load(string path);
double areaID(Mat binaryImg16S);
double areaHolesID(Mat image_binary);


Mat_<double> Load(string path)
{
	// load image
	Mat image = imread(path, CV_LOAD_IMAGE_COLOR);

	Mat image_gray;
	cvtColor(image, image_gray, CV_BGR2GRAY);

	Mat image_binary;
	threshold(image_gray, image_binary, 200, 1, CV_THRESH_BINARY);

	Mat image_binary_inverse;
	threshold(image_gray, image_binary_inverse, 200, 1, CV_THRESH_BINARY_INV);

	Mat image_binary_16s;
	image_binary_inverse.convertTo(image_binary_16s, CV_16S);

	// collect data
	Mat blobs;
	int numberOfHoles = numberOfHolesID(image_binary);
	vector<vector<Point>> contours;
	allContours(image_binary_inverse, contours);
	double energy = bendingEnergyID(image_binary_16s);
	return (Mat_<double>(1, 2) << numberOfHoles , energy);
}

void loadSquareImageTrainingSet(Mat& ITset, Mat& OTset)
{
	// create input set
	ITset.push_back(Load("./../Images/trainingSquare.png"));
	ITset.push_back(Load("./../Images/trainingCircle.png"));
	printMatrix(ITset);

	// create desired output
	OTset = (Mat_<double>(2, 1) << 0
		, 1);

	string c;
	getline(cin, c);
}

const double MAX_OUTPUT_ERROR = 1E-10;
const int MAXRUNS = 10000;

int main(int argc, char** argv)
{
	Mat ITset, OTset;
	loadSquareImageTrainingSet(ITset, OTset);

	Mat V0, W0, dW0, dV0;

	int hiddenNeurons = 2;

	cout << " ===> BPN format: " << endl <<
		"BPN Inputlayer  = " << ITset.cols << "  neurons" << endl <<
		"BPN Outputlayer = " << OTset.cols << "  neurons" << endl << endl;
	cout << "Please choose a number of hidden neurons: ";
	cin >> hiddenNeurons;

	cout << "Initialize BPN ..." << endl;
	initializeBPN(ITset.cols, hiddenNeurons, OTset.cols, V0, dV0, W0, dW0);
	cout << "Press ENTER => ";
	string dummy;
	getline(cin, dummy);
	getline(cin, dummy);

	Mat IT, OT, OH, OO;
	double outputError0, outputError1, sumSqrDiffError = MAX_OUTPUT_ERROR + 1;
	Mat V1, W1;
	int runs = 0;
	while ((sumSqrDiffError > MAX_OUTPUT_ERROR) && (runs < MAXRUNS))
	{
		sumSqrDiffError = 0;

		for (int inputSetRowNr = 0; inputSetRowNr < ITset.rows; inputSetRowNr++)
		{
			IT = transpose(getRow(ITset, inputSetRowNr));
			OT = transpose(getRow(OTset, inputSetRowNr));
			calculateOutputHiddenLayer(IT, V0, OH);
			calculateOutputBPN(OH, W0, OO);
			adaptVW(OT, OO, OH, IT, W0, dW0, V0, dV0, W1, V1);
			calculateOutputBPNError(OO, OT, outputError0);
			calculateOutputBPNError(BPN(IT, V1, W1), OT, outputError1);
			sumSqrDiffError += (outputError1 - outputError0) * (outputError1 - outputError0);
			V0 = V1;
			W0 = W1;
		}
		cout << "sumSqrDiffError = " << sumSqrDiffError << endl;
		runs++;
	}

	cout << "BPN Training is ready!" << endl << endl;
	cout << "Runs = " << runs << endl << endl;

	Mat inputVectorTrainingSet, outputVectorTrainingSet, outputVectorBPN;
	cout << setw(1) <<
		" Training Input" << setw(12) <<
		"| Expected Output " << setw(1) <<
		"| Output BPN " << setw(6) <<
		"|" << endl << endl;
	for (int row = 0; row < ITset.rows; row++)
	{
		inputVectorTrainingSet = transpose(getRow(ITset, row));
		for (int r = 0; r < inputVectorTrainingSet.rows; r++)
			cout << setw(8) << getEntry(inputVectorTrainingSet, r, 0);
		cout << setw(2) << "|";

		outputVectorTrainingSet = transpose(getRow(OTset, row));

		for (int r = 0; r < outputVectorTrainingSet.rows; r++)
			cout << setw(8) << round(getEntry(outputVectorTrainingSet, r, 0));
		cout << setw(2) << "|";
		outputVectorBPN = BPN(inputVectorTrainingSet, V0, W0);
		for (int r = 0; r < outputVectorBPN.rows; r++)
			cout << setw(8) << getEntry(outputVectorBPN, r, 0);
		cout << setw(2) << "|";
		cout << endl;
	}
	Mat test = (Mat_<double>(2, 1) << 1 , 1);
	Mat result = BPN(test, V0, W0);
	cout << "results : " << endl;
	for (int row = 0; row < result.rows; row++)
	{
		for (int col = 0; col < result.cols; col++)
		{
			cout << getEntry(result, row, col);
		}
		cout << endl;
	}
	cout << endl << endl << "Press ENTER for exit";
	getline(cin, dummy);
	getline(cin, dummy);


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

	Mat src = imread("./../Images/trainingset/horcrux2.jpg", CV_LOAD_IMAGE_COLOR);
	Rect r(Point(60, 68), Point(450, 456));//


	/*bounding_box.push_back(left_border);
	bounding_box.push_back(right_border);
	bounding_box.push_back(top_border);
	bounding_box.push_back(bottom_border);*/
//	src = src(r);
//	Mat gray;
//	cvtColor(src, gray, CV_BGR2GRAY);
//	Mat binaryImg;
//	Mat binaryImgINV;
//	threshold(gray, binaryImg, 200, 255, THRESH_BINARY);
//	threshold(gray, binaryImgINV, 200, 1, THRESH_BINARY_INV);
//	Mat binaryImg16S;
//	binaryImgINV.convertTo(binaryImg16S, CV_16S);
//	imshow("src", src);
//	imshow("gray", gray);
//	imshow("binaryImg", binaryImg);
//	waitKey(0);
//	vector<vector<Point>>* contours = new vector<vector<Point>>();
//	allContours(binaryImg16S, *contours);
//	Mat test;
//	drawPoints(binaryImg, test, contours);
//	imshow("test", test);
//	cout << "area : " << contourArea(contours->at(0));
//
//	double areaholes = areaHolesID(binaryImg);
//	cout << "areaholesid " << areaholes << endl;
//	cout << "areaid" << areaID(binaryImg16S) << endl;

	waitKey(0);
}

double areaHolesID(Mat image_binary)
{
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	double area = 0;
	int count = 0;
	findContours(image_binary, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < contours.size(); i = hierarchy[i][0])
	{
		Rect r = boundingRect(contours[i]);
		if (hierarchy[i][2] < 0)
		{
			area += r.area();
			count++;
		}
	}
	return area / count;
}

double areaID(Mat binaryImg16S)
{
	vector<vector<Point>>* contours = new vector<vector<Point>>();
	allContours(binaryImg16S, *contours);
	double area = contourArea(contours->at(0));
	cout << "area : " << area << endl;
	return area;
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

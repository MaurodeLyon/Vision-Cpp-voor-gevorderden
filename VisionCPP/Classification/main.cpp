#include "stdafx.h"

using namespace cv;
using namespace std;

int nrOfHolesID(Mat src, Mat binaryImg);
double bendingEnergyID(Mat binaryImg16S);

void loadSquareImageTrainingSet(Mat& ITset, Mat& OTset)
{
	Mat image_gray;
	cvtColor(imread("./../Images/trainingSquare.png", CV_LOAD_IMAGE_COLOR), image_gray, CV_BGR2GRAY);
	imshow("Gray", image_gray);
	Mat image_binary;
	threshold(image_gray, image_binary, 200, 1, CV_THRESH_BINARY_INV);
	image_binary.convertTo(image_binary, CV_16S);
	//printMatrix(image_binary);
	
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
	//Mat ITset, OTset;
	//loadSquareImageTrainingSet(ITset, OTset);
	
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
	Mat src = imread("./../Images/trainingset/horcrux2.jpg", CV_LOAD_IMAGE_COLOR);
	Rect r(Point(60, 68), Point(450, 456));//
	Rect rect = Rect(68,25,456 -68 + 1,
		488 - 25 + 1);

	/*bounding_box.push_back(left_border);
	bounding_box.push_back(right_border);
	bounding_box.push_back(top_border);
	bounding_box.push_back(bottom_border);*/
	src = src(r);
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


	int holes = nrOfHolesID(src,binaryImg);
	double bendingEnrgy = bendingEnergyID(binaryImg16S);
	
	cout << "H: " << holes << endl;
	cout << "BE:" << bendingEnrgy << endl;
	



	//// Get the camera image
	//Mat image_original = imread("./../Images/square.bmp", CV_LOAD_IMAGE_COLOR);

	//// Detect object
	//Mat image_gray;
	//cvtColor(image_original, image_gray, CV_BGR2GRAY);
	//Mat image_binary;
	//threshold(image_gray, image_binary, 200, 1, CV_THRESH_BINARY_INV);
	//Mat image_binary16S;
	//image_binary.convertTo(image_binary16S, CV_16S);

	//// Extract object 
	////     1. contour
	////     2. flood fill
	////     3. output image
	//compartMentaliseFloodFill(image_original, "figuurFF");
	//// feature detection
	//// Feed the output image to the neural network
	//// Display the results

	//imshow("Original", image_original);
	//show16SImageStretch(image_binary16S, "Binary 16S");
}

int nrOfHolesID(Mat src, Mat binaryImg)
{

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

	return count;
}

double bendingEnergyID(Mat binaryImg16S)
{
	//Bending Energy
	vector<vector<Point>>* contours = new vector<vector<Point>>();
	allContours(binaryImg16S, *contours);
	double bendingEnergyID = 0;
	for (vector<Point> contour : *contours) {
		bendingEnergyID = bendingEnergy(binaryImg16S, contour);
		cout << "bendingEnergy " << bendingEnergyID << endl;
	}
	waitKey(0);

	return bendingEnergyID;
}
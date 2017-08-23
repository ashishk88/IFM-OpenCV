//============================================================================
// Name        : IFM_OpenCV.cpp
// Author      : Ashish Kumar
// Version     : 1.0v
// Copyright   : Your copyright notice
// Description : A c++ program which performs printed label detection and also
//               finds the position, orientation, area and perimeter of the
//               printed label as well as confidence of label detection.
//============================================================================

#include <iostream>
#include <opencv2/opencv.hpp>
#include <ros/ros.h>
#include <ros/master.h>
#include<stdio.h>

using namespace cv;
using namespace std;

/******************************************************************************
* A class to store position of an object
******************************************************************************/
class Position {
private:
	double width;
	double height;
public:
	double getWidth() {
		return width;
	}

	double getHeight() {
		return height;
	}

	Position(double w = 0.0, double h = 0.0) {
		width = w;
		height = h;
	}

};

/******************************************************************************
* A class to store details of a printed label
******************************************************************************/
class PrintedLabel {
private: 
	Position position;
	double orientation;
	double area;
	double perimeter;
	double confidence;

public:	
	Position getPosition() {
		return position;
	}

	double getOrientation() {
		return orientation;
	}

	double getArea() {
		return area;
	}

	double getPerimeter() {
		return perimeter;
	}

	double getConfidence() {
		return confidence;
	}
};

/******************************************************************************
* A class for the detection of printed labels. This class uses the opencv lib-
* rary for the printed label dection and has a function to print details of t-
* he labels.
******************************************************************************/
class PrintedLabelDetector {
private:
	char response;
	Mat image;
	list<PrintedLabel> printedLabels;
public:

	/**************************************************************************
	* A member function which loads image from the hard drive or from a ROS to-
	* pic
	**************************************************************************/
	void loadImage() {
		cout << "Enter (f) for a file and (t) for a ROS topic: ";
		cin >> response;
		if(response == 'f') {
			image = loadImageFromFile();
		} else if (response == 't') {
			image = loadImageFromROSTopic();
		} else {
			cout << "Response is not recognized. Program will be terminated." << endl;
		}
	}

	/**************************************************************************
	* A member function which loads image from the hard drive 
	**************************************************************************/
	Mat loadImageFromFile() {
		string fileName = "";
		while(true) {
			cout << "Enter file name : ";
			// Test if file was read successfully
			if (!std::getline(cin, fileName)) { 
				cout << "I/O Error has occurred";
			}

			// Test if the file exits and the file name is not empty
			if(!fileName.empty()) {
				ifstream f(fileName.c_str());
				if(f.good()) {
					break;
				} else {
					cout << "The file name entered is incorrect.";
				}
			} else {
				cout << "The file name entered is empty.";
			}		
			cout << endl;
		}
		Mat image = imread(fileName, CV_LOAD_IMAGE_COLOR); ;
	
		// Test if the file is supported by the open cv library
		if(!image.data) {
			cout <<  "This file is not supported" << endl ;
		} 
		
		return image;
	}
	
	/**************************************************************************
	* A member function which loads image from a ROS topic
	**************************************************************************/
	Mat loadImageFromROSTopic() {
		Mat image;
		return image;
	}

	/**************************************************************************
	* This function uses opencv apis for the labels detection. The algorithm is
	* as follows:
	* 1) Apply the sharpening filter on the image.
	* 2) Detect all the rectangles in the image.
	* 3) For a single rectangle, perform a histogram check of its color distri-
	*    to ensure that it lies in the range of white and black.
	* 4) Repeat step (3) for each of the rectangles.
	**************************************************************************/
	void processImage() {
		cout << "Started processing" << endl;
		sharpenImage(image);
		showImage(image);
		return;
	}

	/**************************************************************************
	* Used a sharpening filter on the original image.
	* Source : https://en.wikipedia.org/wiki/Kernel_(image_processing)
	**************************************************************************/
	void sharpenImage(Mat &image) {	
		Mat sharpened;
		Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
		filter2D(image, sharpened, image.depth(), kernel);
		image = sharpened;
	}

	void showImage(Mat &image) {
		namedWindow("Image", WINDOW_AUTOSIZE);
		imshow("Image", image); 	
		waitKey(0); 
	}

	void publishResults() {
		cout << "The results are as follows: " << endl;
		list<PrintedLabel>::iterator it;
		int counter = 1;
		for (it = printedLabels.begin(); it != printedLabels.end(); ++it){
			cout << counter << ")";
			cout << "\t" << "(" << it->getPosition().getWidth() << ", " << it->getPosition().getHeight() << ")";
			cout << "\t" << it->getOrientation();
			cout << "\t" << it->getArea();
			cout << "\t" << it->getPerimeter();
			cout << "\t" << it->getConfidence();
		}
	}
};

int main() {
	PrintedLabelDetector pld;
	pld.loadImage();
	pld.processImage();
	pld.publishResults();
	return 0;
}

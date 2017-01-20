#include <iostream>

// Include openCV libraries
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

extern "C" void init_cpp(void); // Display a message when c/cpp cross compiling went fine
extern "C" void init_cv(void);  // Display a message when openCV is correctly initialized

void init_cpp(void) {
cout << "C++ embedding in place." << endl;
} 

void init_cv(void) {
	// Define image matrices	
    	Mat image;	// Raw image to be acquired
	cout << "OpenCV in place." << endl;
}
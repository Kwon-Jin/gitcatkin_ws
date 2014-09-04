#include <iostream>
#include <string>
#include <sstream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
//#include "config.h"
#include "cellcount.h"
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

/// Global variables

int threshold_value = 0;
int threshold_type = 0;
int const max_value = 255;
int const max_type = 4;
int const max_BINARY_value = 255;

Mat imOrigG, imG, dst, imThresh;
char* window_name = "Threshold Demo";

char* trackbar_type = "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
char* trackbar_value = "Value";

/// Function headers
void Threshold_Demo( int, void* );

int main(int argc, char **argv)
{
//cv::Ptr<cv::FeatureDetector> detector = cv::FeatureDetector::create("SimpleBlob");

cv::SimpleBlobDetector::Params params; 
params.thresholdStep = 2;
params.minThreshold = 32;
params.maxThreshold = 64;
//params.filterByColor = true;		// filter my blobs by color
params.blobColor = 255;				// select white blobs
params.minDistBetweenBlobs = 1.0;  // minimum 10 pixels between blobs
params.filterByArea = true;         // filter my blobs by area of blob
params.minArea = 10.0;              // min 20 pixels squared
params.maxArea = 500.0;             // max 500 pixels squared
params.filterByCircularity = false; // dont filter by circularity
params.filterByConvexity = false; 	//
//params.minConvexity = 1;
//params.maxConvexity = 0;

SimpleBlobDetector myBlobDetector(params);
std::vector<cv::KeyPoint> myBlobs;

    //load image
    Mat imOrigG = imread("/home/denise/ROSworkspace/rosbuild_workspace/trial1/src/bacteriacounttest.tiff", CV_LOAD_IMAGE_GRAYSCALE);

    imG = imOrigG.clone();

    // Create a window to display results
    namedWindow( window_name, CV_WINDOW_AUTOSIZE );

    // Create Tracker to choose type of Threshold

    createTrackbar( trackbar_type,
                  window_name, &threshold_type,
                  max_type, Threshold_Demo );

    createTrackbar( trackbar_value,
                  window_name, &threshold_value,
                  max_value, Threshold_Demo );

    /// Call the function to initialize
	//int threshold_selected_value = 0;
    Threshold_Demo( 0, 0 );
	
    /// Wait until user finishes program
    /*while(true)
    {
        int c;
        c = waitKey( 20 );
        if( (char)c == 27 )
        { break; }
    }*/

	// Ask user to input desired threshold value
	string input = "";	
	int myNumber = 0;
	int myStream = 0;

	cout << "Enter threshold value for binarizing image with white blobs for cells:\n>";
 	
 	//manual input
 	myNumber = 51;
 	
 	while (false) {
   		cout << "Please enter a valid number: ";
   		getline(cin, input);
     	// cout << "Input" << input << endl;
   		// This code converts from string to number safely.
   		stringstream myStream(input);
   	if (myStream >> myNumber)
     	break;
   	cout << "Invalid number, please try again" << endl;
 	} //end while

 	cout << "Threshold: " << myNumber << endl << endl;

	// find blobs and store x-y center
		
		// initialize blob detection parameters
		char* blob_window = "Blob Centers";
		char* thresh_window = "Thresholded Image";
		int threshold_im_val = myNumber;
		
		cout << "Compute imThresh at desired threshold..." << endl;
		threshold( imG, imThresh, threshold_im_val, max_BINARY_value, 0);

		cout << "Find blobs using SimpleBlobDetector..." << endl;
	
	// detect
	vector<KeyPoint> keypoints;
	//detector->detect(imThresh, keypoints);
	myBlobDetector.detect(imG, keypoints);
	vector<Point2d> keypoints_vis;
	keypoints_vis.resize(keypoints.size());
	// extract the x y coordinates of the keypoints: 

	//for (int i=0; i<keypoints.size(); i++){
		//keypoints_vis[i].x = keypoints[i].pt.x;
 		//keypoints_vis[i].y = keypoints[i].pt.y;
	//}
	
	


	// Create a window to display results
	cout << "Display thresholded image and original image" << endl;
	drawKeypoints(imThresh, keypoints, imThresh,Scalar::all(-1),4);
    namedWindow( blob_window, CV_WINDOW_AUTOSIZE );
    namedWindow( thresh_window, CV_WINDOW_AUTOSIZE );
cout<<"Number of blobs:"<<keypoints.size()<<endl;
	// Show original image
    imshow(blob_window, imOrigG);	
    imshow(thresh_window, imThresh);
	
	// Plot x-y on image
	


    /// Wait until user finishes program
    while(true)
    {
        int c;
        c = waitKey( 20 );
        if( (char)c == 27 )
        { break; }
    }
}

    /* display image;
    imwrite(IMAGES_PATH "imThresh.jpg", imThresh);
    cout << "image write complete" << endl;
    namedWindow("thresh",WINDOW_FLAGS);
    imshow("thresh",imThresh,);
    waitKey(0);
    */
/**
 * @function Threshold_Demo
 */
void Threshold_Demo( int, void* )
{
  /* 0: Binary
     1: Binary Inverted
     2: Threshold Truncated
     3: Threshold to Zero
     4: Threshold to Zero Inverted
   */

  threshold( imG, dst, threshold_value, max_BINARY_value,threshold_type );

  imshow( window_name, dst );
  cout << "Threshold_value is " << threshold_value << '\n';
}

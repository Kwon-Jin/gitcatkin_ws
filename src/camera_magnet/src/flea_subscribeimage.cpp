#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

Mat imageOrig;
Mat imageRect;
cv_bridge::CvImagePtr cv_ptr;

Mat cameraMatrix,distCoeffs;

// FOR READING CAMERA MSG
void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
	try
	{
	//Convert ROS image to OpenCV type mono8.
	 cv::imshow("view", cv_bridge::toCvShare(msg, "mono8")->image);
	 cout << "Troubleshoot1";
	 cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
	}
	catch (cv_bridge::Exception& e)
	{
	 ROS_ERROR("Could not convert from '%s' to 'mono8'.", msg->encoding.c_str());
	}
}

int main(int argc, char **argv)
{
	//READ camera calibration parameters
    cout << endl << "Reading camera params file: "<< endl;
	const string inputSettingsFile = "camera_cal.yml";
	FileStorage fs;
	fs.open(inputSettingsFile, FileStorage::READ); // Read the settings

	int itNr;
    fs["iterationNr"] >> itNr;
    itNr = (int) fs["iterationNr"];
    cout << itNr;
    if (!fs.isOpened())
    {
        cerr << "Failed to open " << inputSettingsFile << endl;
        //help(av);
        return 1;
    }

    FileNode n = fs["strings"];                         // Read string sequence - Get node
    /*if (n.type() != FileNode::SEQ)
    {
        cerr << "strings is not a sequence! FAIL" << endl;
        return 1;
    }*/

    FileNodeIterator itt = n.begin(), itt_end = n.end(); // Go through the node
    for (; itt != itt_end; ++itt)
        cout << (string)*itt << endl;

    	//int image_width,
    	int image_width,image_height;
        Mat cameraMatrix, distortion_coefficients, rectification_matrix, projection_matrix;

        cout << "Debug" << endl;

        fs["image_width"] >> image_width;                                   
        fs["image_height"] >> image_height;
        fs["camera_matrix"] >> cameraMatrix;                                    // Read cv::Mat
        fs["distortion_coefficients"] >> distortion_coefficients;
        fs["rectification_matrix"] >> rectification_matrix;
        fs["projection_matrix"] >> projection_matrix;
        
        cout << "image_width = " << image_width << endl
             << "image_height = " << image_height << endl
        	 << "cameraMatrix = " << endl << cameraMatrix << endl
        	 << "distortion_coefficients = " << endl << distortion_coefficients << endl
        	 << "rectification_matrix = " << endl << rectification_matrix << endl
        	 << "projection_matrix = " << endl << projection_matrix << endl;
 	///// END READIN GOF YAML FILE ///////////



	ros::init(argc, argv, "image_listener");
	// Create ROS generic subscriber (or publisher)
	ros::NodeHandle nh; 

	// Create OpenCV display window
	cv::namedWindow("Undistorted Image");
	cv::namedWindow("view");

	cv::startWindowThread();

	//Initialize ImageTransport instance with NodeHandle
	image_transport::ImageTransport it(nh);

	//Subscribe to the camera/image base topic. Call imageCallback when new image arrives, with a queue size of 1.
	image_transport::Subscriber sub=it.subscribe("camera/image_raw", 1,imageCallback);


	//Process image:
	if (cv_ptr->image.rows>60 && cv_ptr->image.cols > 60)
			cv::circle(cv_ptr->image,cv::Point(50,50),10,CV_RGB(255,0,0));

	//Display image in window:
	//cv::imshow("Undistorted Image", cv_ptr->image);
	//cv::imshow("Undistorted Image",imageRect);
	ros::spin();
	cv::destroyWindow("view");
	cv::destroyWindow("Undistorted Image");
}

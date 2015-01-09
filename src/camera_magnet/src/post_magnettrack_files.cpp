#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/objdetect/objdetect.hpp>

using namespace std;
using namespace cv;

int main (int, char**)
{
  cout << "main" << endl;
  //VideoCapture cap ("~/bagfiles/inout1/jan72015_120fps_b/frame0%3d.jpg");  // Open the file
  VideoCapture cap ("/home/denise/bagfiles/inout1/jan82015_120fps_b/frame%04d.jpg");  // Open the files 

  cout << "VideoCapture" << endl;    
  if (!cap.isOpened ())               // Check if opening was successful
    cerr << "I have failed!" << endl;
  
  namedWindow( "image", CV_WINDOW_AUTOSIZE );
  namedWindow( "Blobs image", CV_WINDOW_AUTOSIZE );

  for(;;)
  {// time keeping
    clock_t t;
    int f;
    t = clock();
    cout << "frame:" << cap.get(CV_CAP_PROP_POS_FRAMES) << endl;
    //char* imageName = argv[1];

    Mat image;
  
    //image = imread( imageName, CV_LOAD_IMAGE_GRAYSCALE );
    cap >> image; 
    cvtColor(image, image, CV_BGR2GRAY);  
    cout << "Image size:" << image.size() << "Channels: " << image.channels() << endl; //TROUBLESHOOT
    
    if(!image.data ) //check if image exists
    {
      cout << " No image data " << endl;
      return -1;
    }
    
    imshow("image",image);      // display image


    // find magnets
    cv::SimpleBlobDetector::Params params;
    params.minThreshold = 0;
    params.maxThreshold = 50;
    params.thresholdStep = 10;

    params.minArea = 20; 
    params.minConvexity = 0.8;
    params.minInertiaRatio = 0.01;

    params.maxArea = 1000;
    params.maxConvexity = 10;
    //params.maxInertiaRatio = 8;
    params.blobColor = 0;
    params.filterByCircularity = false;
    // ... any other params you don't want default value
    // set up and create the detector using the parameters

    SimpleBlobDetector blob_detector( params );
    blob_detector.create("SimpleBlob");  

    // detect!
    vector<KeyPoint> keypoints;
    vector< vector<Point> > contours;
    vector< vector<Point> > approxContours;


    /* // MASK NOT WORKING
    Mat mask = Mat::zeros(image.size(), CV_8U);  // type of mask is CV_8U
    Mat roi(mask, Rect(1,270,630,50));
    roi = cv::Scalar(255,255,255);

    namedWindow("Mask", CV_WINDOW_AUTOSIZE);
    imshow("Mask", mask);
    cout<< "Mask size: " << mask.size() << "Channels: " << mask.channels() << endl;
    cv::waitKey(0);

    blob_detector.detect(image, keypoints, mask);
    */

    blob_detector.detect(image, keypoints);
    for (int i=0; i<keypoints.size(); i++){
      float X=keypoints[i].pt.x; 
      float Y=keypoints[i].pt.y;
      cout << "x = " << keypoints[i].pt.x << ", y = " << keypoints[i].pt.y << "size = " << keypoints[i].size << endl;
    }
    Mat out;
    drawKeypoints( image, keypoints, out, CV_RGB(0,255,0), DrawMatchesFlags::DEFAULT);

    //imshow( imageName, image );
    imshow( "Blobs image", out );

    t = clock() - t;

    float tsec = ((float)t)/CLOCKS_PER_SEC;

    cout<<"It took me "<< t << " clicks (" << tsec <<" seconds)" << endl;

    waitKey(10);
    //if(waitKey(30) >= 0) break;
  }
  return 0;
}

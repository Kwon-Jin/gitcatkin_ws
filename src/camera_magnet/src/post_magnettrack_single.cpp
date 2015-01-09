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

/*// Begin: The main function to load video and analyse image for magnets
int main (int argc, char *argv[])
{

VideoCapture cap ("/home/denise/bagfiles/inout1/images/output.mpg");  // Open the file
    
if (!cap.isOpened ())               // Check if opening was successful
    cerr << "I have failed!" << endl;

else
{
    //
    Mat edges;
    Mat frame;
    namedWindow("edges",1);
    while (cap.read (frame))
    {
      cvtColor(frame, edges, CV_BGR2RGB);  
      imshow("edges", frame);
      if(waitKey(30) >= 0) break;
    }
}

  if( argc != 2 || !image.data )
 {
   printf( " No image data \n " );
   return -1;
 }

 Mat gray_image;
 cvtColor( image, gray_image, CV_BGR2GRAY );

 imwrite( "../../images/Gray_Image.jpg", gray_image );

 namedWindow( imageName, CV_WINDOW_AUTOSIZE );
 namedWindow( "Gray image", CV_WINDOW_AUTOSIZE );

 imshow( imageName, image );
 imshow( "Gray image", gray_image );

 waitKey(0);

return 0;

}
// end video analysis*/

// Begin: Analyse 1 image



int main( int argc, char** argv)
{
  // time keeping
  clock_t t;
  int f;
  t = clock();

 char* imageName = argv[1];

  Mat image;
  //image = imread("/home/denise/bagfiles/inout1/jan62015_120fps/frame0043.jpg")
  image = imread( imageName, CV_LOAD_IMAGE_GRAYSCALE );
  cout << "Image size:" << image.size() << endl;
  if( argc != 2 || !image.data )
  {
    cout << " No image data " << endl;
    return -1;
  }

  // convert image to grayscale
  //Mat gray_image;
  //cvtColor( image, gray_image, CV_BGR2GRAY );

  
 
  // post process gray_image:
  // simpleblobdetector
  // set up the parameters (check the defaults in opencv's code in blobdetector.cpp)
  cv::SimpleBlobDetector::Params params;
  params.minThreshold = 1;
  params.maxThreshold = 100;
  params.thresholdStep = 5;

  params.minArea = 20; 
  params.minConvexity = 0.8;
  params.minInertiaRatio = 0.01;

  params.maxArea = 1000;
  params.maxConvexity = 10;

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
  blob_detector.detect(image, keypoints);
  

  for (int i=0; i<keypoints.size(); i++){
    float X=keypoints[i].pt.x; 
    float Y=keypoints[i].pt.y;
  }
  Mat out;
  drawKeypoints( image, keypoints, out, CV_RGB(0,255,0), DrawMatchesFlags::DEFAULT);
  //approxContours.resize( contours.size() );

  /*for( int i = 0; i < contours.size(); ++i )
  {
    approxPolyDP( Mat(contours[i]), approxContours[i], 4, 1 );
    drawContours( out, contours, i, CV_RGB(rand()&255, rand()&255, rand()&255) );
    drawContours( out, approxContours, i, CV_RGB(rand()&255, rand()&255, rand()&255) );
  }
  cout << "Keypoints " << keyPoints.size() << endl;
  */      
  
  // Save image
  //imwrite( "../../images/Gray_Image.jpg", gray_image );
  namedWindow( imageName, CV_WINDOW_AUTOSIZE );
  namedWindow( "Blobs image", CV_WINDOW_AUTOSIZE );

  imshow( imageName, image );
  imshow( "Blobs image", out );

  t = clock() - t;

  float tsec = ((float)t)/CLOCKS_PER_SEC;

  cout<<"It took me "<< t << " clicks (" << tsec <<" seconds)" << endl;
  
  waitKey(0);

  return 0;
}
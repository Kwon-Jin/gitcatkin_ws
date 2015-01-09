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

// for ROS - Opencv
#include <ros/ros.h>
#include <image_transport/image_transport.h> 
#include <cv_bridge/cv_bridge.h> 
#include <sensor_msgs/image_encodings.h>
#include <camera_magnet/xyReal.h>

using namespace std;
using namespace cv;

static const std::string OPENCV_WINDOW = "Image window";

/* magnet_track.cpp : Subscribes to '/camera/image_raw', uses simpleblobdetector to identify magnet.
publishes vector x,y-posn in camera coordinates of magnets to xyReal
TO ADD: 
  - calibration at the beginning of script to identify ROI
  - mask using calibration ROI
  - intelligent filter for position of magnets
*/

//http://wiki.ros.org/cv_bridge/Tutorials/UsingCvBridgeToConvertBetweenROSImagesAndOpenCVImages
class ImageConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;
  ros::Publisher xyMagnet_pub_;
public:
  ImageConverter()
    : it_(nh_)
  {
    // Subscribe to input video feed and publish output video feed
    image_sub_ = it_.subscribe("/camera/image_raw", 1, 
      &ImageConverter::imageCb, this);
    image_pub_ = it_.advertise("/magnet_track/output_video", 1);

    //publish vector of magnet positions
    xyMagnet_pub_ = nh_.advertise<camera_magnet::xyReal>("/magnet_track/xyReal",1);
    
    cv::namedWindow(OPENCV_WINDOW);
  }

  ~ImageConverter()
  {
    cv::destroyWindow(OPENCV_WINDOW);
  }

  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    /*// time keeping : initialize
    clock_t t;
    int f;
    t = clock();
  */
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::MONO8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }

    
    // Live track magnets using simpleblobdetector
    // blob detector parameters
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

    // set up and create the detector using the parameters
    SimpleBlobDetector blob_detector( params );
    blob_detector.create("SimpleBlob");  

    // detect!
    vector<KeyPoint> keypoints;
    blob_detector.detect(cv_ptr->image, keypoints);
    
    /*// time keeping
    t = clock() - t;
    float tsec = ((float)t)/CLOCKS_PER_SEC;
    */
    cout << " ====== " << endl;
    //cout<<"It took me "<< t << " clicks (" << tsec <<" seconds)" << endl;
    // end time keeping
    int midx [2] = {0,1};
    int count = 0;

    // cycle through keypoints
    for (int i=0; i<keypoints.size(); i++){ 
      // determine which keypoints are magnets as filtered by posn
      int ymin = 300;
      int ymax = 310;

      if (keypoints[i].pt.y < ymax && keypoints[i].pt.y > ymin)
      {
      // candidate keypoints for magnet
      midx[count] = i;
      count = count + 1;
      float X=keypoints[i].pt.x; 
      float Y=keypoints[i].pt.y;
      cout << "x = " << keypoints[i].pt.x << ", y = " << keypoints[i].pt.y << " size = " << keypoints[i].size << endl;
      Point pt;
      pt.x = keypoints[i].pt.x; 
      pt.y = keypoints[i].pt.y; 
      circle( cv_ptr->image, pt , 10 , CV_RGB(0,0,255), 3 , -1 );
      }
    }
    drawKeypoints( cv_ptr->image, keypoints,  cv_ptr->image, CV_RGB(0,255,0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    
    //cout << " ==tb==== " << endl;
    cout << "midx : " << midx[0] << "  " << midx[1] << endl;

//////////// FINISH ////////////////////
// if more then 2 candidate keypoints, filter for 2 that 
    
    
    // Update GUI Window
    //cv::imshow(OPENCV_WINDOW, cv_ptr->image);
    cv::imshow(OPENCV_WINDOW,  cv_ptr->image);

    cv::waitKey(3);

    // Output modified video stream
    image_pub_.publish(cv_ptr->toImageMsg());
    

    // package position for xyReal.msg:
    camera_magnet::xyReal xymsg;
    
    if (keypoints.size()<1)
      cout<<"No blobs detected.  Turn on light?" << endl;
    else{
      // check which is left and which is right
      if (keypoints[midx[0]].pt.x > keypoints[midx[1]].pt.x){
        xymsg.leftx = keypoints[midx[1]].pt.x;
        xymsg.lefty = keypoints[midx[1]].pt.y;
        xymsg.rightx = keypoints[midx[0]].pt.x;
        xymsg.righty = keypoints[midx[0]].pt.y;
      }
      else{
        xymsg.leftx = keypoints[midx[0]].pt.x;
        xymsg.lefty = keypoints[midx[0]].pt.y;
        xymsg.rightx = keypoints[midx[1]].pt.x;
        xymsg.righty = keypoints[midx[1]].pt.y;      
      }
    }
    //cout<< "xleft: "<< keypoints[1].pt.x << endl;
    //cout<< "xright: "<< keypoints[2].pt.x << endl;

    // Output position vector
    xyMagnet_pub_.publish(xymsg);
  }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "magnet_track"); // initialize node: magnet_track
  //ros::init(argc, argv, "magnetpose_real");

  ImageConverter ic;
  
  ros::spin();
  return 0;
}

//////////////////////////////////////
  
/* // time keeping
    clock_t t;
    int f;
    t = clock();
    // .... do stuff ...
    t = clock() - t;

    float tsec = ((float)t)/CLOCKS_PER_SEC;

    cout<<"It took me "<< t << " clicks (" << tsec <<" seconds)" << endl;
*/

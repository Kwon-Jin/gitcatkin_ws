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
//#pragma once
// for ROS - Opencv
#include <ros/ros.h>
#include <image_transport/image_transport.h> 
#include <cv_bridge/cv_bridge.h> 
#include <sensor_msgs/image_encodings.h>
#include <camera_magnet/xyReal.h>
#include "hw4.h"

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
    
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      //cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::MONO8);
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::RGB8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }

    // 0. Capture image?
    cout << "Move setup until straw is horizontal and fills field of view."<< endl;
    cv::waitKey(0);
    //Mat image(3,3,CV_32F,Scalar(5));
    // 1, Identify y-limits for track
    cout << "Click on yMIN of straw"<< endl;
    Point ymin = getClick("image",cv_ptr->image);
    cout << "ymin: " << ymin << endl;
    drawCross(cv_ptr->image, ymin, 5);
    waitKey(10);

    cout << "Click on yMAX of straw"<< endl;
    Point ymax = getClick("image",cv_ptr->image);
    cout << "ymax: " << ymax << endl;
    drawCross(cv_ptr->image, ymax, 5);
    waitKey(10);

    // 2. Identify x limits
    cout << "Click on xMIN of straw"<< endl;
    Point xmin = getClick("image",cv_ptr->image);
    cout << "xmin: " << xmin << endl;
    drawCross(cv_ptr->image, ymax, 5);
    waitKey(10);

    cout << "Click on xMIN of straw"<< endl;
    Point xmax = getClick("image",cv_ptr->image);
    cout << "xmax: " << xmax << endl;
    drawCross(cv_ptr->image, ymax, 5);
    waitKey(10);
    

    double L = 0.1; 
    cout << "L(m):" << L << endl;
    bool  updateYaml = true;
    // Write Yaml file
    if(updateYaml){
      cout<< "Updating YAML" << endl;

      FileStorage fs("cal.yml",FileStorage::WRITE);
      if(ymin.y<ymax.y)
        fs << "ymin" << ymin.y << "ymax" << ymax.y;
      else
        fs << "ymin" << ymax.y << "ymax" << ymin.y;
      fs << "xmin" << xmin.x << "xmax" << xmax.x;
      fs << "L" << L ;
      fs.release();
      
    }
    else{
      cout<< "Don't update YAML" << endl;
      
    }




    // Update GUI Window
    //cv::imshow(OPENCV_WINDOW, cv_ptr->image);
    cv::imshow(OPENCV_WINDOW,  cv_ptr->image);

    cv::waitKey(3);



    // Output modified video stream
    image_pub_.publish(cv_ptr->toImageMsg());
    

    // package position for xyReal.msg:
    camera_magnet::xyReal xymsg;
    
    //cout<< "xleft: "<< keypoints[1].pt.x << endl;
    //cout<< "xright: "<< keypoints[2].pt.x << endl;

    // Output position vector
    xyMagnet_pub_.publish(xymsg);
  }
};

int main(int argc, char** argv)
{
  cout << "======================================" << endl;
  cout << "Calibration script for magnet_track.cpp" << endl;
  cout << "======================================" << endl;
  ros::init(argc, argv, "magnet_track"); // initialize node: magnet_track
  //ros::init(argc, argv, "magnetpose_real");

  ImageConverter ic;
  
  ros::spin();
  return 0;
}
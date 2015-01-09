#include <ros/ros.h>


#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include "sensor_msgs/Image.h"

#include "flea.h"
flea_to_cv flea_camera;


void publish_camera_data(ros::Publisher * pub) {
	 cv_bridge::CvImagePtr cv_ptr;
   cv_ptr.reset(new cv_bridge::CvImage);

	 cv::Mat color, black;
   // flea_camera.getFrame(color,black);
    try
    {
      // cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
      flea_camera.getFrame(cv_ptr->image);
      // flea_camera.getFrame(black);
      // cv_ptr->image = black;
      // cv_ptr->encoding =  sensor_msgs::image_encodings::TYPE_8SC3;
      cv_ptr->encoding =  sensor_msgs::image_encodings::MONO8;

      sensor_msgs::ImagePtr ros_img = cv_ptr->toImageMsg();
      ros_img->header.stamp = ros::Time::now();
      pub->publish(ros_img);
      // pub->publish(cv_ptr->toImageMsg());
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }
}



// class ImageConverter
// {
//   ros::NodeHandle nh_;
//   image_transport::ImageTransport it_;
//   image_transport::Subscriber image_sub_;
//   image_transport::Publisher image_pub_;
  
// public:
//   ImageConverter()
//     : it_(nh_)
//   {
//     // Subscrive to input video feed and publish output video feed
//     image_sub_ = it_.subscribe("/camera/image_raw", 1, 
//       &ImageConverter::imageCb, this);
//     image_pub_ = it_.advertise("/image_converter/output_video", 1);

//     cv::namedWindow(OPENCV_WINDOW);
//   }

//   ~ImageConverter()
//   {
//     cv::destroyWindow(OPENCV_WINDOW);
//   }

//   void imageCb(const sensor_msgs::ImageConstPtr& msg)
//   {
//     cv_bridge::CvImagePtr cv_ptr;
//     try
//     {
//       cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
//     }
//     catch (cv_bridge::Exception& e)
//     {
//       ROS_ERROR("cv_bridge exception: %s", e.what());
//       return;
//     }

//     // Draw an example circle on the video stream
//     if (cv_ptr->image.rows > 60 && cv_ptr->image.cols > 60)
//       cv::circle(cv_ptr->image, cv::Point(50, 50), 10, CV_RGB(255,0,0));

//     // Update GUI Window
//     cv::imshow(OPENCV_WINDOW, cv_ptr->image);
//     cv::waitKey(3);
    
//     // Output modified video stream
//     image_pub_.publish(cv_ptr->toImageMsg());
//   }
// };

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  

  ros::NodeHandle nh;
  // image_transport::ImageTransport it_;
  // image_transport::Subscriber image_sub_;
  ros::Publisher image_pub = nh.advertise<sensor_msgs::Image>("/camera/image_raw", 1);
  // ros::Publisher image_pub = nh.advertise<sensor_msgs::ImageConstPtr>("/image_converter/output_video", 1);
  // image_transport::ImageTransport it_;
  // image_transport::Subscriber image_sub_;
  // image_transport::Publisher image_pub_;


  while(nh.ok()) {
    // ros::Time prior = ros::Time::now();

  	publish_camera_data(&image_pub);

    // ROS_ERROR("dt %f",  (ros::Time::now()-prior).toSec()*1000.0  );
  	ros::spinOnce();
  }
  return 0;
}
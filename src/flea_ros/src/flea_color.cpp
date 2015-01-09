#include <ros/ros.h>


#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include "sensor_msgs/Image.h"

#include "flea.h"
#include <iostream>

flea_to_cv::flea_to_cv() {
	
	FlyCapture2::Error err;
	
	FlyCapture2::BusManager busMgr;
	FlyCapture2::PGRGuid guid;
	FlyCapture2::Property prop;

	prop.type = FlyCapture2::GAIN;
	prop.autoManualMode = false; // don't do this outside
	//Ensure the property is set up to use absolute value control.
	prop.absControl = true;
	//Set the absolute value of gain to 10.5 dB.
	prop.absValue = 10.5;



	busMgr.GetCameraFromIndex(0, &guid);

	FlyCapture2::VideoMode pVideoMode;
	FlyCapture2::FrameRate pFrameRate;

	err = cam.Connect(&guid);
	cam.GetVideoModeAndFrameRate(&pVideoMode, &pFrameRate );
	std::cout << "Mode " << pVideoMode << std::endl;
	std::cout << "Frame " << pFrameRate << std::endl;
	// FlyCapture2::IPP
	// err = cam.SetVideoModeAndFrameRate( FlyCapture2::VIDEOMODE_640x480Y8 , FlyCapture2::FRAMERATE_120 );
	cam.SetProperty( &prop );
	std::cout << "Error " << err.GetDescription() << std::endl;

	std::cout << "Connected " << cam.IsConnected() << std::endl;
	cam.StartCapture();


	image.SetColorProcessing(FlyCapture2::IPP);
	image.SetDefaultColorProcessing(FlyCapture2::IPP);
}
flea_to_cv::~flea_to_cv() {	
	cam.StopCapture();
	cam.Disconnect();
}

bool flea_to_cv::getFrame(cv::Mat &bw) {


	
	// std::cout << "Connected " << cam.IsConnected() << std::endl;
	// std::cout << "getting buffer" << std::endl;

	cam.RetrieveBuffer(&image);
	cv::Mat cv_img ( image.GetRows(),image.GetCols(), CV_8UC3,image.GetData());

	// std::cout << "got buffer" << std::endl;
	cv_img.copyTo(bw);
	
	
	return true;
}

static flea_to_cv flea_camera;


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
      cv_ptr->encoding =  sensor_msgs::image_encodings::RGB8;

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
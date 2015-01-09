#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>

using namespace cv;
using namespace std;

static const std::string OPENCV_WINDOW = "Image window";
Mat camera_matrix, distortion_coefficients, rectification_matrix, projection_matrix;

class ImageConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;
  
public:
  ImageConverter()
    : it_(nh_)
  {
    // Subscrive to input video feed and publish output video feed
    image_sub_ = it_.subscribe("/camera/image_raw", 1, 
      &ImageConverter::imageCb, this);
    image_pub_ = it_.advertise("/image_converter/output_video", 1);

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
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }

    // Draw an example circle on the video stream
    //if (cv_ptr->image.rows > 60 && cv_ptr->image.cols > 60)
    //  cv::circle(cv_ptr->image, cv::Point(50, 50), 10, CV_RGB(255,0,0));

    // Rectify image
    cv::Mat view, rview, map1, map2;
    Size imageSize;
    imageSize = cv_ptr->image.size();
    //cout<<"Troubleshoot";
    initUndistortRectifyMap(camera_matrix, distortion_coefficients, Mat(), getOptimalNewCameraMatrix(camera_matrix, distortion_coefficients, imageSize, 1, imageSize, 0), imageSize, CV_16SC2, map1, map2);
    remap(cv_ptr->image,rview,map1,map2, INTER_LINEAR);


    // Update GUI Window
    cv::imshow(OPENCV_WINDOW, rview);
    cv::waitKey(3);
    
    // Output modified video stream
    image_pub_.publish(cv_ptr->toImageMsg());
  }
};

int main(int argc, char** argv)
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
      //Mat camera_matrix, distortion_coefficients, rectification_matrix, projection_matrix;

        cout << "Debug" << endl;

        fs["image_width"] >> image_width;                                   
        fs["image_height"] >> image_height;
        fs["camera_matrix"] >> camera_matrix;                                    // Read cv::Mat
        fs["distortion_coefficients"] >> distortion_coefficients;
        fs["rectification_matrix"] >> rectification_matrix;
        fs["projection_matrix"] >> projection_matrix;
        
        cout << "image_width = " << image_width << endl
             << "image_height = " << image_height << endl
           << "camera_matrix = " << endl << camera_matrix << endl
           << "distortion_coefficients = " << endl << distortion_coefficients << endl
           << "rectification_matrix = " << endl << rectification_matrix << endl
           << "projection_matrix = " << endl << projection_matrix << endl;
  ///// END READIN GOF YAML FILE ///////////


  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  ros::spin();
  return 0;
}
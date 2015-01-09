#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "flycapture/FlyCapture2.h"
#include "flycapture/FlyCapture2GUI.h"

class flea_to_cv {
	public:

	flea_to_cv();
	~flea_to_cv();

	// bool getFrame(cv::Mat &color, cv::Mat &bw);
	bool getFrame(cv::Mat &bw);

	
	private:
		FlyCapture2::Camera cam;
		FlyCapture2::Image image,c_image;


};
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
	cv::Mat cv_img ( image.GetRows(),image.GetCols(), CV_8UC1,image.GetData());

	// std::cout << "got buffer" << std::endl;
	cv_img.copyTo(bw);
	
	
	return true;
}

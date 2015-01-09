#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>

//#define IMAGE_PATH "/home/denise/catkin_ws/src/mbrcellcount/images/"
#define IMAGE_PATH "/home/denise/bagfiles/inout1/jan72015_120fps_b/"

using namespace cv;
using namespace std;

Mat src; Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);
ofstream cellposn;

// input parameters to set
string filename = "frame0107";
string filenameread = filename + ".jpg";
string filenametxt = filename + ".txt";
string filenamewrite = filename + "_regionSelection.jpg";

// cell detection parameters:
int contoursizelowbound = 5; //defaults: 5

/// Function header
void thresh_callback(int, void* );

/** @function main */
int main( int argc, char** argv )
{
  
//  cellposn.open (filenametxt);
  cellposn.open ("filenametxt.txt");
  /// Load source image and convert it to gray
  src = imread(IMAGE_PATH+filenameread,CV_LOAD_IMAGE_GRAYSCALE);
  //inRange(src, Scalar(0,0,0), Scalar(10,0,0),  src);
  cout << "Image size:" << src.size() << endl;
  /// Show in a window
  namedWindow( "src", CV_WINDOW_AUTOSIZE );
  imshow( "src", src );
  //src_gray = src.clone();
  
  /// Convert image to gray and blur it
  //cvtColor( src, src_gray, CV_BGR2GRAY );
  //threshold( src_gray, src_gray, 112, 255,3 );
  //blur( src_gray, src_gray, Size(3,3) );
  
  /// Sharpen image using Bilateral filter
  int filteridx = 3;
  bilateralFilter(src, src_gray, filteridx, filteridx*2,filteridx/2, BORDER_DEFAULT );
  
  /// Create Window
  char* source_window = "Source";
  namedWindow( source_window, CV_WINDOW_AUTOSIZE );
  imshow( "src", src_gray );
  imshow( source_window, src_gray );
  
  createTrackbar( " Canny thresh:", "Source", &thresh, max_thresh, thresh_callback );
  thresh_callback( 0, 0 );
  
  waitKey(0);
  return(0);
}

/** @function thresh_callback */
void thresh_callback(int, void* )
{
  Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  
  /// Detect edges using canny
  Canny( src_gray, canny_output, thresh, thresh*2, 3 );
  /// Find contours
  findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
  
  // find rotated rectangles and ellipses for each contour
  vector<RotatedRect> minRect( contours.size() );
  vector<RotatedRect> minEllipse( contours.size() );
  vector<double> ratio( contours.size() );
  vector<double> size( contours.size() );
  
  for( int i = 0; i < contours.size(); i++ )
  { 
	minRect[i] = minAreaRect( Mat(contours[i]) );
  
  	if( contours[i].size() > contoursizelowbound )
	{ 
		minEllipse[i] = fitEllipse( Mat(contours[i]) );
  		cout<< "contour" << endl;
  		Point2f ellipse_points[4]; minEllipse[i].points( ellipse_points );
		double majorAxis = sqrt(pow(ellipse_points[0].x - ellipse_points[1].x,2) + pow(ellipse_points[0].y - ellipse_points[1].y,2));
		double minorAxis = sqrt(pow(ellipse_points[0].x - ellipse_points[3].x,2) + pow(ellipse_points[0].y - ellipse_points[3].y,2));
		ratio[i] = majorAxis/minorAxis;
		size[i] = majorAxis*minorAxis;
  
	  	//cout<< "ratio = " << ratio[i] << endl; 
	  	//cout<< "size = " << size[i] << endl; 
	}
  
  }
  
  /// Draw contours + rotated rects + ellipses
  Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
  Mat drawingContours = Mat::zeros( canny_output.size(), CV_8UC3 );
  Mat drawingCells;
  cvtColor( src, drawingCells, CV_GRAY2BGR );

  for( int i = 0; i< contours.size(); i++ )
  {
    Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
    drawContours( drawingContours, contours, i, color, 1, 8, hierarchy, 0, Point() );
    //if( ratio[i] > 2 )
    if( size[i] < 300 && size[i] > 20)
      //&& ratio[i]>1.5 )
    {
      cout<< "ratio = " << ratio[i] << endl; 
      cout<< "size = " << size[i] << endl; 
      // Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
      drawContours( drawing, contours, i, color, 1, 8, hierarchy, 0, Point() );
      // ellipse
      ellipse( drawing, minEllipse[i], color, 1, 8 );
      // rotated rectangle
      //Point2f rect_points[4]; minRect[i].points( rect_points );
      //for( int j = 0; j < 4; j++ )
      //   line( drawing, rect_points[j], rect_points[(j+1)%4], color, 1, 8 );
      
      // Draw a line in center of ellipse (major axis over original)
      // Determine location of points of cell and store in  
      Point2f ellipse_points[4]; minEllipse[i].points( ellipse_points );
      Point2f midpt[2]; 
      midpt[0].x = (ellipse_points[0].x + ellipse_points[3].x)/2;
      midpt[0].y = (ellipse_points[0].y + ellipse_points[3].y)/2;
      midpt[1].x = (ellipse_points[1].x + ellipse_points[2].x)/2;
      midpt[1].y = (ellipse_points[1].y + ellipse_points[2].y)/2;
            
      // Point2f cellpts; cellpoints[
      line( drawingCells, midpt[0], midpt[1], color, 2,8);
      
      // Write position of cell in file
      // write coordinates for each cell's major axis as [x1 y1 x2 y2] 
      cellposn << midpt[0].x <<" "<< midpt[0].y << " " << midpt[1].x <<" " << midpt[1].y <<endl;

    }
  }
  
  /// Close textfile
  cellposn.close();
   
  
  
  /// Show in a window
  //namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
  //imshow( "Contours", drawing );
  
  // display Canny param and number of contours detected
  cout <<"Canny = " << thresh << endl;
  int numContours = contours.size();
  cout <<"numContours = " << numContours << endl;
  
  /// Show in a window
  namedWindow( "Filtered Contours", CV_WINDOW_AUTOSIZE );
  imshow( "Filtered Contours", drawing );
  namedWindow( "All Contours", CV_WINDOW_AUTOSIZE );
  imshow( "All Contours", drawingContours );
  namedWindow( "Cells", CV_WINDOW_AUTOSIZE );
  imshow( "Cells", drawingCells );
  
  imshow( "Source" , canny_output);
  


  /// save image;
  imwrite(IMAGE_PATH+filenamewrite, drawingCells);
  cout << "image write complete" << endl;  
}

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include "config.h"
//#include "cellcount.h"
#include <opencv2/core/core.hpp>

//#include </usr/lib/libgtest_main.a>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{

  Mat center = imread("/home/denise/ROSworkspace/rosbuild_workspace/trial1/src/im_000031.tif", CV_LOAD_IMAGE_COLOR);

  namedWindow("center", WINDOW_FLAGS);

  imshow("center", center);

  waitKey(2);
  cout << "Press 'q' or ESC to quit..." << endl;
  
  int key;
  bool looping = true;
  while(looping) {
    key = waitKey(0);
    switch(key & 0x7F) {
      case 'q': // fall through
     case 'Q': // fall through
      case 27:  // ESC key
        looping = false;
    }
  }

  return 0;
}

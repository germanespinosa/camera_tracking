#ifndef IMAGE_STITCHING_INCLUDE_GAURD
#define IMAGE_STITCHING_INCLUDE_GAURD

#include <vector>

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/stitching/detail/matchers.hpp"
#include "opencv2/calib3d/calib3d.hpp"


using namespace cv;
using std::vector;
using std::string;

class imageStitcher
{
  public:
  vector<Point2f> pointsMapping;
  Size opSize;
  Size finalSize;
  vector<Mat> allHomographies;
  int min_x, max_x, min_y, max_y;
  Size chessboardDims;
  bool doLensCorrection;
  string lensCorrectionFolderPath;


  imageStitcher(string configPath="./config/camera_homographies/", bool doLensCorrection_=true,
		string lensCorrectionFolderPath_="./config/camera_intrinsics_1024x1024");

  Mat stitchImagesOnline(vector<Mat> images);
};


#endif

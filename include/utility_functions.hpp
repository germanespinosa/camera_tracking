// Author: Senthil Palanisamy
// A file describing some utility functions used across all the other scripts

#ifndef UTILITY_FUNCTIONS_INCLUDE_GAURD
#define UTILITY_FUNCTIONS_INCLUDE_GAURD
#include <cell_world.h>
#include <iostream>

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using std::vector;
//using cv::detail::MatchesInfo;
using std::cout;
using std::string;
using std::tuple;

extern Mat outputImage;



struct ImageStitchData
{
  Mat dstImage;
  Mat inputImage;
  Mat homography;
};


Mat stitchImageschessBoard(Mat stitchedImage, Mat ipImage, Mat Homography);
void* WarpandStitchImages(void *arguments);

void performLensCorrection(Mat& image, int imageNo, string lensCorrectionFolderPath);
string return_date_header();
string getFolderPath();
string return_date_time_header();
tuple<vector<double>, Mat> readCameraParameters(string jsonFilePath);
string return_time_header();


vector<double> getContoursArea(vector<vector<cv::Point>> &contours);
vector<int> getDetections(vector<double> &areas, double lower_bound, double upper_bound);
vector<int> getDetections(vector<vector<cv::Point>> &contours, double lower_bound, double upper_bound);


class Coordinates_associator
{

public:
    vector<vector<int>> cell_centers;
    vector<vector<int>> cell_index;
    Coordinates_associator(string fileName);
    cell_world::Coordinates get_coordinates(int mice_x, int mice_y);
};


#endif


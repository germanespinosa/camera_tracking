#include <image_processing.h>
#include <iostream>
#include "xcliball.h"
#include "opencv2/calib3d/calib3d.hpp"

using namespace std;
using namespace json_cpp;

Image_processing::Image_processing(const string &config_file_path, const string &cameras_config_file) {
    pxd_PIXCIopen("", "", config_file_path.c_str());
    if (pxd_goLive(15, 1)){
        cerr << "Failed to initialize frame grabbers" << endl;
        exit(1);
    }
    cv::Size image_dimensions {pxd_imageXdim(), pxd_imageYdim()};
    Json_vector<Camera::Parameters> cameras_parameters;
    cameras_parameters.load(cameras_config_file);
    for (int camera_number = 0; camera_number<cameras_parameters.size(); camera_number ++) {
        cameras.emplace_back(cameras_parameters[camera_number], camera_number, image_dimensions);
    }
}

void Image_processing::capture_all(bool perform_correction) {
    for (auto &camera : cameras){
        camera.capture(perform_correction);
    }
}

Camera::Camera(Parameters &params, unsigned int camera_number, cv::Size &image_dimensions) :
    grabber_bit_map(1<<camera_number),
    frame_size ( image_dimensions.width  * image_dimensions.height)
{
    distCoeffs = vector<double>(5);
    cameraMatrix = cv::Mat(3, 3, CV_64F);
    buffer = (unsigned char *)malloc(frame_size);
    for(int i=0; i < distCoeffs.size(); ++i)
        distCoeffs[i] = params.dist[0][i];
    for(int i=0; i < 3; ++i)
        for(int j=0; j < 3; ++j )
            cameraMatrix.at<double>(i, j)= params.intrinsic[i][j];
    cv::getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, image_dimensions, 0, image_dimensions, 0);

    raw_image = cv::Mat( image_dimensions.height, image_dimensions.width, CV_8UC1, buffer , 0 );
    correction_matrix = cv::getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, image_dimensions, 0, image_dimensions, 0);
}

void Camera::capture( bool perform_correction ) {
    auto i = pxd_readuchar( grabber_bit_map, 1,0, 0,-1, -1, buffer, frame_size,"Grey");
    corrected = false;
    if (perform_correction) correct();
}

cv::Mat &Camera::get_image( bool perform_correction ) {
    if (perform_correction) {
        if (!corrected) correct();
        return corrected_image;
    }
    return raw_image;
}

cv::Mat &Camera::get_new_image( bool perform_correction ) {
    capture(perform_correction);
    return get_image(perform_correction);
}

void Camera::correct() {
    cv::undistort( raw_image, corrected_image, cameraMatrix, distCoeffs, correction_matrix );
    corrected = true;
}

Camera::~Camera() {
    //if (buffer) free(buffer);
}

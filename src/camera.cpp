#include <camera.h>
#include <iostream>
#include "xcliball.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace std;
using namespace json_cpp;

unsigned int Camera::camera_count = 0;
cv::Size Camera::image_size = {0,0};

Image_processing::Image_processing(const string &config_file) {
    Image_processing_parameters ipp;
    ipp.load(config_file);
    Camera::init(ipp.configuration_file);
    for (int camera_number = 0; camera_number < ipp.camera_count; camera_number ++) {
        cameras.emplace_back();
        stringstream ss;
        ss << ipp.backgrounds_folder << "/camera_" << camera_number << ".png";
        cv::Mat background = cv::imread(ss.str());
        cameras.back().set_background(background);
    }
}

void Image_processing::capture_all() {
    for (auto &camera : cameras){
        camera.capture();
    }
}

Image_processing::~Image_processing() {
    Camera::close();
}

void Image_processing::save_images(const string &path) {
    capture_all();
    for (int i =0; i < cameras.size();i++){
        stringstream ss;
        ss << path << "/camera_" << i << ".png";
        cv::imwrite(ss.str(), cameras[i].image);
    }
}

Camera::Camera() :
    grabber_bit_map(1 << Camera::camera_count)
{
    Camera::camera_count++;
    frame_size = Camera::image_size.width  * Camera::image_size.height;
    buffer = (unsigned char *)malloc(frame_size);
    image = cv::Mat(Camera::image_size.height, Camera::image_size.width, CV_8UC1, buffer , 0 );
}

void Camera::capture( ) {
    auto i = pxd_readuchar( grabber_bit_map, 1,0, 0,-1, -1, buffer, frame_size,"Grey");
    is_subtracted = false;
}

cv::Mat &Camera::get_image(bool new_image) {
    if (new_image) capture();
    return image;
}

void Camera::init(string &config_file_path) {
    pxd_PIXCIopen("", "", config_file_path.c_str());
    if (pxd_goLive(15, 1)){
        cerr << "Failed to initialize frame grabbers" << endl;
        exit(1);
    }
    Camera::image_size = {pxd_imageXdim(), pxd_imageYdim()};
    cout << Camera::image_size.width << "," << Camera::image_size.height << endl;
}

void Camera::close() {
    pxd_PIXCIclose();
    cout << "EPIX cameras closed" << endl;
}

cv::Mat &Camera::get_subtracted(bool new_image) {
    if (new_image) capture();
    subtract();
    return subtracted;
}

void Camera::subtract(){
    if(background.empty()) throw logic_error("background not set");
    if (is_subtracted) return;
    cv::absdiff(image, background, subtracted);
    is_subtracted = true;
}

void Camera::set_background(cv::Mat background_image) {
    cv::cvtColor(background_image, background, cv::COLOR_BGR2GRAY);
}

#include <sys/stat.h>
#include <camera.h>
#include <iostream>
#include "xcliball.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace std;
using namespace json_cpp;

unsigned int Camera::camera_count;
cv::Size Camera::image_size;

Image_processing::Image_processing(const string &config_file) {
    parameters.load(config_file);
    Camera::init(parameters.configuration_file);
    for (int camera_number = 0; camera_number < parameters.camera_count; camera_number ++) {
        cameras.emplace_back();
        stringstream ss;
        ss << parameters.backgrounds_folder << "/camera_" << camera_number << ".png";
        cameras.back().load_background(ss.str());
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
        cameras[i].save_image(ss.str());
    }
}

void Image_processing::save_backgrounds() {
    save_images(parameters.backgrounds_folder);
}

Camera::Camera() :
    grabber_bit_map(1 << Camera::camera_count),
    camera_number(Camera::camera_count)
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

void Camera::init(const string &config_file_path) {
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

void Camera::set_background(const cv::Mat& background_image) {
    cv::cvtColor(background_image, background, cv::COLOR_BGR2GRAY);
}

void Camera::save_image(const string &file_path) {
    cv::imwrite(file_path, image);
}

void Camera::load_background(const std::string &file_path) {
    background_path = file_path;
    struct stat buffer;
    if  (stat (file_path.c_str(), &buffer) == 0) {
        cv::Mat bg = cv::imread(file_path);
        set_background(bg);
    }
}

void Camera::save_background() {
    save_image(background_path);
}

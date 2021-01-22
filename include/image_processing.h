#pragma once
#include <json_cpp.h>
#include <string>
#include <vector>
#include "opencv2/core.hpp"

struct Camera {
    struct Parameters :json_cpp::Json_object {
        Json_object_members(
                Add_member (dist);
                Add_member(intrinsic);
                Add_optional_member(homography);
                Add_optional_member(extrinsic);
        )
        json_cpp::Json_vector<json_cpp::Json_vector<double>> dist;
        json_cpp::Json_vector<json_cpp::Json_vector<double>> homography;
        json_cpp::Json_vector<json_cpp::Json_vector<double>> intrinsic;
        json_cpp::Json_vector<json_cpp::Json_vector<double>> extrinsic;
    };
    Camera(Parameters &,unsigned int, cv::Size &);
    ~Camera();
    std::vector<double> distCoeffs;
    cv::Mat cameraMatrix;
    unsigned int grabber_bit_map;
    unsigned int frame_size;
    unsigned char *buffer;
    cv::Mat raw_image;
    cv::Mat correction_matrix;
    cv::Mat corrected_image;
    bool corrected;
    void capture(bool perform_correction = true);
    void correct ();
    cv::Mat &get_image(bool perform_correction = true);
    cv::Mat &get_new_image(bool perform_correction = true);
};

struct Image_processing {
    Image_processing (const std::string&, const std::string&);
    void capture_all(bool perform_correction = true);
    std::vector<Camera> cameras;
};
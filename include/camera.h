#pragma once
#include <json_cpp.h>
#include <string>
#include <vector>
#include "opencv2/core.hpp"

struct Camera {
    static void init(const std::string &);
    static void close();
    Camera();
    void load_background(const std::string &);
    void set_background(const cv::Mat&);
    void capture();
    void subtract();
    cv::Mat &get_image(bool new_image = true);
    cv::Mat &get_subtracted(bool new_image = true);
    void save_image(const std::string &);
    void save_background();
    int grabber_bit_map;
    unsigned int frame_size;
    unsigned char *buffer;
    cv::Mat image;
    cv::Mat subtracted;
    bool is_subtracted = false;
    cv::Mat background;
    std::string background_path;
    unsigned int camera_number;
private:
    static unsigned int camera_count;
    static cv::Size image_size;
};

struct Image_processing_parameters : json_cpp::Json_object{
    Json_object_members(
            Add_member(camera_count);
            Add_member(configuration_file);
            Add_optional_member(backgrounds_folder);
            )
    std::string configuration_file;
    unsigned int camera_count;
    std::string backgrounds_folder;
};

struct Image_processing {
    Image_processing (const std::string&);
    ~Image_processing();
    void capture_all();
    void save_images(const std::string &);
    void save_backgrounds();
    Image_processing_parameters parameters;
    std::vector<Camera> cameras;
};
#pragma once
#include <core.h>
#include <cell_world.h>
#include <camera.h>
#include "opencv2/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/cudabgsegm.hpp"
#include "opencv2/video.hpp"
#include "opencv2/highgui.hpp"
#include <agent_profile.h>

struct Detection : json_cpp::Json_object {
    unsigned int camera;
    std::string agent;
    Pixel_location location;
    Json_object_members(
            Add_member(camera);
            Add_member(agent);
            Add_member(location);
    )
};

struct Camera_manager {
    Camera_manager (Camera &, Agent_profiles &);
    Camera & camera;
    Agent_profiles &profiles;
    std::vector<Detection>get_detections(cv::Mat &);
};

struct Agent_detection {
    Agent_detection(std::vector<Camera>&, Agent_profiles &);
    std::vector<Detection> get_detections ();
    std::vector<Camera_manager> camera_managers;
};
#pragma once
#include <cell_world.h>
#include <camera.h>
#include "simple_capture.hpp"

#include "opencv2/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/cudabgsegm.hpp"
#include "opencv2/video.hpp"
#include "opencv2/highgui.hpp"

using Pixel_location = cell_world::Location;

struct Coordinate_solver : json_cpp::Json_object {
    struct Association : json_cpp::Json_object {
        Pixel_location centroid;
        cell_world::Coordinates coordinates;
        double square_distance (Pixel_location &);
        Json_object_members(
                Add_member(centroid);
            Add_member(coordinates);
        )
    };
    struct Camera_association : json_cpp::Json_object {
        json_cpp::Json_vector<Association> associations;
        Json_object_members(
        Add_member(associations);
        )
        cell_world::Coordinates get_coordinates(Pixel_location &);
    };
    json_cpp::Json_vector<Camera_association> camera_associations;
    Json_object_members(
            Add_member(camera_associations);
            )
};

struct Agent_profile : json_cpp::Json_object {
    std::string agent_name;
    int lower_bound;
    int upper_bound;
    Json_object_members(
        Add_member(agent_name);
        Add_member(lower_bound);
        Add_member(upper_bound);
    )
    bool match(double);
};

struct Agent_profiles : json_cpp::Json_object {
    std::vector<Agent_profile> get_matching_profiles(double);
    json_cpp::Json_vector<Agent_profile> profiles;
    Json_object_members(
            Add_member(profiles);
            )

};

struct Detection : json_cpp::Json_object {
    std::string agent;
    cell_world::Coordinates coordinates;
    Json_object_members(
            Add_member(agent);
            Add_member(coordinates);
    )
};

struct Connected_components {
    struct Connected_components_configuration : json_cpp::Json_object{
        Coordinate_solver coordinate_solver;
        Agent_profiles agent_profiles;
    };
    bool update_last_detection(const Detection &);
    std::vector<Detection> last_detections;
};

struct Camera_manager {
    Camera_manager (Camera &, Agent_profiles &,Coordinate_solver::Camera_association &);
    Camera & camera;
    Agent_profiles &profiles;
    Coordinate_solver::Camera_association &associations;
    std::vector<Detection>get_detections();
};

struct Agent_detection {
    Agent_detection(std::vector<Camera>&, Agent_profiles &, Coordinate_solver &);
    std::vector<Detection> get_detections ();
    std::vector<Camera_manager> camera_managers;
};
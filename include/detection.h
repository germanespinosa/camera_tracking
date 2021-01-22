#pragma once
#include <cell_world.h>
#include "simple_capture.hpp"

struct Coordinate_solver : json_cpp::Json_object {
    struct Association : json_cpp::Json_object {
        cell_world::Coordinates centroid;
        cell_world::Coordinates coordinates;
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
        cell_world::Coordinates get_coordinates(cell_world::Coordinates detection);
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
    bool match(int);
};

struct Agent_profiles : json_cpp::Json_object {
    json_cpp::Json_vector<Agent_profile> profiles;
    std::string match (int);
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

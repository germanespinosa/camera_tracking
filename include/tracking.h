#pragma once
#include <detection.h>
#include <cell_association.h>
#include <cell_world.h>

struct Agent_coordinates{
    std::string agent_name;
    cell_world::Coordinates coordinates;
};
struct Tracking {
    Tracking(const std::string &, const std::string &, const std::string &);
    Image_processing ip;
    Agent_profiles profiles;
    Agent_detection detection;
    Coordinate_solver coordinate_solver;
    std::vector<Agent_coordinates> get_agents_coordinates();
};
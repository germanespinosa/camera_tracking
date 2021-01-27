#pragma once
#include <core.h>
#include <cell_world.h>
#include <detection.h>

struct Coordinate_solver : json_cpp::Json_object {
    struct Association : json_cpp::Json_object {
        Pixel_location centroid;
        cell_world::Coordinates cell_coordinates;
        double square_distance (Pixel_location &);
        Json_object_members(
        Add_member(centroid);
        Add_member(cell_coordinates);
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
    cell_world::Coordinates get_coordinates(Detection &);
    Json_object_members(
    Add_member(camera_associations);
    )
};
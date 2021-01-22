#pragma once
#include <cell_world.h>

struct Controller{
    struct Configuration : json_cpp::Json_object {
        std::string address;
        int port;
        Json_object_members(
                Add_member(address);
                Add_member(port);
                )
    };
    Controller();
    int track_agent(std::string, cell_world::Coordinates);
    Configuration configuration;
};

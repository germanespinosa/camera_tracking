#pragma once
#include <cell_world.h>

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

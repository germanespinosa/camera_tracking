#include<detection.h>
#include <fstream>

using namespace std;

bool Agent_profile::match(int area) {
    return ( area >= lower_bound &&
         (upper_bound == 0 || area <= upper_bound));
}

std::string Agent_profiles::match(int area) {
    for (auto &profile: profiles){
        if (profile.match(area)) return profile.agent_name;
    }
    return "";
}

bool Connected_components::update_last_detection(const Detection &detection) {
    for (auto &last_detection : last_detections)
        if (last_detection.agent == detection.agent){
            if (last_detection.coordinates == detection.coordinates) return false;
            last_detection.coordinates = detection.coordinates;
            return true;
        }
    last_detections.push_back(detection);
    return true;
}


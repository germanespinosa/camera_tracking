#include <detection.h>
#include <fstream>
#include <opencv2/aruco.hpp>

using namespace std;
using namespace cell_world;

bool Agent_profile::match(double area) {
    return ( area >= lower_bound &&
         (upper_bound == 0 || area <= upper_bound));
}

std::vector<Agent_profile> Agent_profiles::get_matching_profiles(double area) {
    std::vector<Agent_profile> matching_profiles;
    for (auto &profile: profiles){
        if (profile.match(area)) matching_profiles.push_back(profile);
    }
    return matching_profiles;
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

double Coordinate_solver::Association::square_distance(Pixel_location &detection) {
    return  pow(detection.x - centroid.x,2) + pow(detection.y - centroid.y,2);
}

Coordinates Coordinate_solver::Camera_association::get_coordinates(Pixel_location &detection) {
    double min_sq_distance = - 1;
    Coordinates coordinates;
    for (auto &a : associations){
        auto sq_distance = a.square_distance(detection);
        if (min_sq_distance == -1 || sq_distance < min_sq_distance){
            min_sq_distance = sq_distance;
            coordinates = a.coordinates;
        }
    }
    return coordinates;
}

Camera_manager::Camera_manager(Camera &camera, Agent_profiles &profiles, Coordinate_solver::Camera_association &associations) :
    camera(camera),
    profiles(profiles),
    associations(associations){
}

std::vector<Detection> Camera_manager::get_detections() {
    cv::Mat bw = ((camera.get_subtracted(false) > 50) * 255);
    cv::Mat labels;
    cv::Mat stats;
    cv::Mat centroids;
    int nLabels = connectedComponentsWithStats(bw,labels,stats,centroids,8);
    vector<Detection> detections;
    for (int i = 0; i<centroids.rows ;i++){
        int area =  stats.at<int>(4, i);
        auto matching_profiles = profiles.get_matching_profiles(area);
        if (!matching_profiles.empty()) {
            Pixel_location centroid {centroids.at<double>(0, i), centroids.at<double>(1, i)};
            auto coord = associations.get_coordinates(centroid);
            for (auto &profile : matching_profiles) {
                Detection d;
                d.agent = profile.agent_name;
                d.coordinates = coord;
                detections.push_back(d);
            }
        }
    }
    return detections;
}

Agent_detection::Agent_detection(vector<Camera> &cameras, Agent_profiles &profiles, Coordinate_solver & coordinate_solver) {
    int i = 0;
    for (auto &camera : cameras){
        camera_managers.emplace_back(camera,profiles,coordinate_solver.camera_associations[i]);
        i++;
    }
}

std::vector<Detection> Agent_detection::get_detections() {
    vector<Detection> detections;
    for (auto &cm : camera_managers)
    {
        auto cd = cm.get_detections();
        for (auto d : cd) detections.push_back(d);
    }
    return detections;
}

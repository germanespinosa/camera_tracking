#include <detection.h>
#include <fstream>
#include <opencv2/aruco.hpp>

using namespace std;
using namespace cell_world;

Camera_manager::Camera_manager(Camera &camera, Agent_profiles &profiles) :
    camera(camera),
    profiles(profiles){
}

std::vector<Detection> Camera_manager::get_detections(cv::Mat &subtracted) {
    cv::Mat bw = subtracted > 50;
    cv::Mat eroded;
    cv::erode(bw, eroded, cv::Mat(), cv::Point(-1, -1), 2, 1, 1);
    cv::Mat dilated;
    cv::dilate(eroded, dilated, cv::Mat(), cv::Point(-1, -1), 10, 1, 2);
    cv::Mat centroids;
    cv::Mat labels;
    cv::Mat stats;
    int nLabels = connectedComponentsWithStats(dilated,labels,stats,centroids,8);
    vector<Detection> detections;
    for (int i = 0; i< stats.rows; i++)
    {
        int area = stats.at<int>(i,4);
        double x = centroids.at<double>(i, 0);
        double y = centroids.at<double>(i, 1);
        auto matching_profiles = profiles.get_matching_profiles(area);
        if (!matching_profiles.empty()) {
            cv::Rect roi;
            roi.x = x;
            roi.y = y;
            roi.width = stats.at<int>(i, 2);
            roi.height = stats.at<int>(i, 3);
            Pixel_location location {x,y};
            for (auto &p : matching_profiles){
                Detection detection;
                detection.camera = camera.camera_number;
                detection.agent = p.agent_name;
                detection.location = location;
                detections.push_back(detection);
            }
        }
    }
    return detections;
}

Agent_detection::Agent_detection(vector<Camera> &cameras, Agent_profiles &profiles) {
    int i = 0;
    for (auto &camera : cameras){
        camera_managers.emplace_back(camera,profiles);
        i++;
    }
}

std::vector<Detection> Agent_detection::get_detections() {
    vector<Detection> detections;
    for (auto &cm : camera_managers)
    {
        auto cd = cm.get_detections(cm.camera.get_subtracted(false));
        for (auto d : cd) detections.push_back(d);
    }
    return detections;
}

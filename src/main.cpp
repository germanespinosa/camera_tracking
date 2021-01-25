// Author: Senthil Palanisamy
// This is the main function that lauches all exection
// 1. Does mice tracking using background subtraction
// 2. Does robot tracking by using the black square placed on top of the robot
// 3. Writes raw images acquired from cameras into a video
// 4. Stitched images together ad writes a stitched video
#include <cell_world.h>
#include <vector>
#include <string>
#include <iostream>
#include <opencv2/aruco.hpp>
#include "backGroundSubtraction.hpp"
#include <json_cpp.h>
#include <controller.h>
#include <detection.h>
#include <camera.h>
#include "xcliball.h"

using namespace cell_world;
using namespace std::chrono;
using namespace json_cpp;
using std::vector;
using std::cout;
using std::ifstream;
using std::cout;
using std::string;
using std::to_string;
using cv::Size;
using cv::Point;
using cv::Scalar;

struct Coordinates_repetitions{
    Coordinates_list coordinates;
    vector<int> repetitions;
    Coordinates get_coordinates(Coordinates_list &candidates){
        for (auto &candidate : candidates){
            bool found = false;
            for (int i = 0; i < coordinates.size(); i++){
                if (coordinates[i] == candidate) {
                    repetitions[i] ++;
                    found = true;
                    break;
                }
            }
            if (!found){
                coordinates.push_back(candidate);
                repetitions.push_back(1);
            }
        }
        int max = -1;
        for (int i = 0; i < coordinates.size(); i++){
            bool found = false;
            for (auto &candidate : candidates) {
                if (coordinates[i] == candidate) {
                    found = true;
                    break;
                }
            }
            if (!found)
                repetitions[i] = 0;
            else
                if ( max == Not_found ||  repetitions[i]>repetitions[max] ) max = i;
        }
        if (max == -1)
            return {1000,1000};
        else
            return coordinates[max];
    }
};

void ccc(cv::Mat &image){
    Mat bw = ((image > 100) * 255);
    Mat labels;
    Mat stats;
    Mat centroids;
    int nLabels = connectedComponentsWithStats(bw,labels,stats,centroids,8);
    //for (int i=0 ; i<centroids.cols);

}

int main(int argc, char **argv)
{
    if (argc == 1) {
        cout << "Missing configuration parameter" << endl;
        return 1;
    }
    string config_file(argv [1]);
    Image_processing ip("./config/"+ config_file + ".config");
    Controller controller;
    Agent_profiles profiles;
    profiles.load("./config/agent_profile.config");
    Coordinate_solver coordinate_solver;
    coordinate_solver.load("./config/coordinate_solver.config");
    vector<Camera_manager> cameras;
    if (argc > 2){
        string p(argv [2]);
        if (p=="-c") {
            string output_path;
            if (argc < 4) {
                output_path = ".";
            } else {
                output_path = argv[3];
            }
            ip.save_images(output_path);
        }
        if (p=="-s") {
            if (argc < 4) {
                cerr << "missing camera number parameter" << endl;
                exit(1);
            }
            string camera_number(argv[3]);
            int cam = stoi(camera_number);
            cout << "showing camera " << cam << " in real time" << endl;
            cv::Size ws{600, 600};
            cv::Mat mouse_cut;
            while (true) { ;
                ip.capture_all();
                cv::Mat subtracted = ip.cameras[cam].get_subtracted(false).clone();

                Mat bw = ((subtracted > 50) * 255);
                cv::Mat resized;
                cv::resize(bw, resized, ws);
                cv::imshow("subtracted", resized);
                if (waitKey(10) > 0) break;
            }
        }
        if (p=="-t"){
            string camera_number (argv[3]);
            cout << "showing camera 0 real time"<< endl;
            int cam = stoi(camera_number);
            cv::Size ws{600,600};
            cv::Mat mouse_cut;
            while (true) { ;
                cv::Mat original = ip.cameras[cam].get_image(true).clone();
                cv::Mat subtracted = ip.cameras[cam].get_subtracted(false).clone();
                Mat bw = ((subtracted > 50) * 255);
                Mat labels;
                Mat stats;
                Mat centroids;
                int nLabels = connectedComponentsWithStats(bw,labels,stats,centroids,8);
                for (int i = 0; i<centroids.rows ;i++){
                    int area =  stats.at<int>(Point(4, i));
                    if (area > 7000 && area < 10000) {
                        double x = centroids.at<double>(0, i);
                        double y = centroids.at<double>(1, i);
                        cout << area << " " << i << " (" << x << "," << y << ")" << endl;
                    }
                }
                cv::Mat resized;
                cv::resize(bw, resized, ws);
                cv::Mat inverted = original.clone();
                inverted = 255;
                inverted = inverted - original;
                cv::imshow("subtracted", resized);
                if (waitKey(500)>0) break;
            }
        }

        pxd_PIXCIclose();
        return 0;
    }
    Coordinates mouse;
    Coordinates_repetitions mouse_repetitions;
    while(true)
    {
        Coordinates_list mouse_candidates;
        ip.capture_all();
        for(auto &camera:cameras)
        {
            auto candidates = camera.get_detections();
            for (auto &c : candidates)
                if (c.agent == "mouse") mouse_candidates.push_back(c.coordinates);
        }
        auto best_mouse = mouse_repetitions.get_coordinates(mouse_candidates);
        if (best_mouse!=Coordinates{1000,1000})
            if (mouse != best_mouse) {
                cout << mouse << endl;
                mouse = best_mouse;
            }
        if(waitKey(2) == 27) break;
    }
    return 0;
}

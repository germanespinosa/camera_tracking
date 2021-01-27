#include <string>
#include <iostream>
#include <opencv2/aruco.hpp>
#include <json_cpp.h>
#include <controller.h>
#include <tracking.h>
#include "xcliball.h"

using namespace cv;
using namespace std;
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
    coordinate_solver.load("./config/camera_associations.config");
    Agent_detection ad(ip.cameras,profiles);
    if (argc > 2){
        string p(argv [2]);
        if (p=="-c") {
            string output_path;
            if (argc < 4) {
                ip.save_backgrounds();
            } else {
                string output_path = argv[3];
                ip.save_images(output_path);
            }
        }
        if (p=="-d") {
            if (argc < 4) {
                cerr << "missing camera number parameter" << endl;
                exit(1);
            }
            string camera_number(argv[3]);
            int cam = stoi(camera_number);
            if (argc < 5) {
                cerr << "missing imagepfile path" << endl;
                exit(1);
            }
            string image_path(argv[4]);
            cv::Mat image = imread(image_path);
            cv::cvtColor(image, ip.cameras[cam].image, cv::COLOR_BGR2GRAY);

            auto ds = ad.camera_managers[cam].get_detections(ip.cameras[cam].get_subtracted(false));
            for (auto d : ds){
                cout << d.agent << " detected at " << d.location << " cell " << coordinate_solver.camera_associations[cam].get_coordinates(d.location) << endl;
                cv::circle(image, Point(d.location.x, d.location.y), 20, Scalar(255, 255, 255), 10, 8, 0);
            }
            for (auto c: coordinate_solver.camera_associations[cam].associations){
                Point p (c.centroid.x , c.centroid.y );

                cv::circle(image, p, 10, Scalar(255, 255, 255), 10, 8, 0);
                string s = std::to_string(c.cell_coordinates.x) + "," + std::to_string(c.cell_coordinates.y);
                cv::putText(image,
                            s,
                            p, // Coordinates
                            cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
                            2, // Scale. 2.0 = 2x bigger
                            cv::Scalar(255,255,255), // BGR Color
                            2); // Anti-alias (Optional)
            }
            cv::Mat resized;
            cv::Size ws{600, 600};
            cv::resize(image, resized, ws);
            cv::imshow("subtracted", resized);
            waitKey(100000);

            return 0;
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
                ip.cameras[cam].capture();
                cv::Mat subtracted = ip.cameras[cam].get_subtracted(false).clone();
                Mat bw = ((subtracted > 50) * 255);
                cv::Mat eroded;
                cv::erode(bw, eroded, cv::Mat(), cv::Point(-1, -1), 2, 1, 1);
                cv::Mat dilated;
                cv::dilate(eroded, dilated, cv::Mat(), cv::Point(-1, -1), 2, 1, 1);

                cv::Mat resized;
                cv::resize(dilated, resized, ws);
                cv::imshow("subtracted", resized);

                if (waitKey(10) > 0) break;
            }
        }
        if (p=="-t"){
            while (true){
                ip.capture_all();
                auto detections = ad.get_detections();
                for (auto &d : detections){
                    cout << d.agent << ": " << d.location << endl;
                }
                if (waitKey(10) > 0) break;
            }
        }

        pxd_PIXCIclose();
        return 0;
    }
    return 0;
}

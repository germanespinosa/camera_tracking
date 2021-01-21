// Author: Senthil Palanisamy
// This is the main function that lauches all exection
// 1. Does mice tracking using background subtraction
// 2. Does robot tracking by using the black square placed on top of the robot
// 3. Writes raw images acquired from cameras into a video
// 4. Stitched images together ad writes a stitched video
#include <cell_world.h>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>

#include <opencv2/aruco.hpp>

#include "backGroundSubtraction.hpp"
#include "utility_functions.hpp"
#include "simple_capture.hpp"
#include "video_recorder.hpp"
#include <json_cpp.h>

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

int update_mouse_position(vector<int> coord){

    struct Response: Json_object{
        Json_object_members(
                Add_member(code);
                Add_member(message);
        )
        int code;
        string message;
    };
    std::stringstream ss;
    cout << "new mouse position " << coord[0] << "," << coord[1] << endl;
    ss << "http://192.168.137.1:8080/track/mouse/" << coord[0] << "/" << coord[1];
    string url = ss.str();
    Json_web_get(url);
    return 1;
}

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

int main()
{
  int cameraCount = 4;
  // Reading all cell association file
  vector<Coordinates_associator> cellAssociation;
  cellAssociation.emplace_back("./config/cell_association/camera0.txt");
  cellAssociation.emplace_back("./config/cell_association/camera1.txt");
  cellAssociation.emplace_back("./config/cell_association/camera2.txt");
  cellAssociation.emplace_back("./config/cell_association/camera3.txt");

  // check if the camera video config fmt file is correct
  frameGrabber imageTransferObj("./config/video_config/new_white_light.fmt");

  //VideoCapture cap(input_video);
  vector<Mat> frame;
  Method method=MOG2;

  vector<BackGroundSubtractor> bgsubs;
  vector<Mat> frames;
  imageTransferObj.transferAllImagestoPC();

  frames.push_back(imageTransferObj.image0);
  frames.push_back(imageTransferObj.image1);
  frames.push_back(imageTransferObj.image2);
  frames.push_back(imageTransferObj.image3);

  string homographyConfigPath = "./config/camera_homographies/";

   string imageSizePath = homographyConfigPath + "image_size.txt";

  // Intialising four background subtractors one for each view of the camera
  for(int i=0; i<cameraCount; ++i)
    bgsubs.emplace_back(method, frames[i], false);

  vector<Vec4i> hierarchy;
  bool first = true;
  Coordinates mouse;
  Coordinates robot;
  Coordinates_repetitions mouse_repetitions;
  Coordinates_repetitions robot_repetitions;
  while(true)
  {
    vector<Mat> foregroundImages;
    vector<Point> selectedContour;
    auto start1 = high_resolution_clock::now();
    imageTransferObj.transferAllImagestoPC();

    frames[0] = imageTransferObj.image0.clone();
    frames[1] = imageTransferObj.image1.clone();
    frames[2] = imageTransferObj.image2.clone();
    frames[3] = imageTransferObj.image3.clone();
    Coordinates_list mouse_candidates;
    Coordinates_list robot_candidates;
    for(int i=0; i < cameraCount; ++i)
    {
        vector<vector<Point> > contours;
        auto foregroundImage = bgsubs[i].processImage(frames[i]);
        findContours( foregroundImage, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
        auto mouse_candidates_id = getDetections(contours, 3500, 5500);
        for (auto id : mouse_candidates_id){
            auto M = moments(contours[id]);
            auto cx = int(M.m10 / M.m00);
            auto cy = int(M.m01 / M.m00);
            mouse_candidates.push_back(cellAssociation[i].get_coordinates(cx, cy));
        }
        auto robot_candidates_id = getDetections(contours, 400, 0);
        for (auto id : robot_candidates_id){
            auto M = moments(contours[id]);
            auto cx = int(M.m10 / M.m00);
            auto cy = int(M.m01 / M.m00);
            robot_candidates.push_back(cellAssociation[i].get_coordinates(cx, cy));
        }
    }
    auto best_mouse = mouse_repetitions.get_coordinates(mouse_candidates);
    if (best_mouse!=Coordinates{1000,1000})
        if (mouse != best_mouse) {
            cout << mouse << endl;
            mouse = best_mouse;
        }
//    robot = robot_repetitions.get_coordinates(robot_candidates);
    if(waitKey(2) == 27) break;
  }
  return 0;
}

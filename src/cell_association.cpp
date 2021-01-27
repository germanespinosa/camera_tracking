#include <cell_association.h>
#include <cmath>

using namespace cell_world;

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
            coordinates = a.cell_coordinates;
        }
    }
    return coordinates;
}

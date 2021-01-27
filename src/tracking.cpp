#include <tracking.h>

using namespace std;
vector<Agent_coordinates> Tracking::get_agents_coordinates() {
    vector<Agent_coordinates> agent_coordinates;
    ip.capture_all();
    auto ds = detection.get_detections();
    for (auto d : ds){
        Agent_coordinates ac;
        ac.agent_name = d.agent;
        ac.coordinates = coordinate_solver.get_coordinates(d);
        agent_coordinates.push_back(ac);
    }
    return agent_coordinates;
}

Tracking::Tracking(const string &camera_configuration_file_path, const string &agents_profiles_config_file_path, const string &camera_cell_association_file_path):
ip(camera_configuration_file_path),
detection(ip.cameras,profiles)
{
    profiles.load(agents_profiles_config_file_path);
    coordinate_solver.load(camera_configuration_file_path);
}


#include <controller.h>

using namespace json_cpp;
using namespace cell_world;
using namespace std;

Controller::Controller() {
    configuration.load("controller.config");
}

int Controller::track_agent(std::string agent, cell_world::Coordinates coord) {
    struct Response: Json_object{
        Json_object_members(
                Add_member(code);
                Add_member(message);
        )
        int code;
        string message;
    } response;
    std::stringstream ss;
    cout << "new "<< agent << " position " << coord.x << "," << coord.y << endl;
    ss << "http://" << configuration.address << ":" << configuration.port << "/track/" << agent << "/" << coord.x << "/" << coord.y;
    string url = ss.str();
    Json_web_get(url).get_stream() >> response;
    return response.code;
}


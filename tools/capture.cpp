#include <string>
#include <iostream>
#include <tracking.h>

using namespace cv;
using namespace std;
using namespace cell_world;
using namespace json_cpp;

int main(int argc, char **argv)
{
    if (argc == 1) {
        cout << "Missing configuration parameter" << endl;
        return 1;
    }
    string config_file(argv [1]);
    Image_processing ip("./config/"+ config_file + ".config");
    string output_path;
    if (argc < 3) {
        cout << "Saving images to backgrounds folder" << endl;
        ip.save_backgrounds();
    } else {
        string output_path = argv[2];
        cout << "Saving images to backgrounds to " << output_path << endl;
        ip.save_images(output_path);
    }
    return 0;
}

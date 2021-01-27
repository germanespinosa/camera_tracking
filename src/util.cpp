#include <util.h>
#include <sys/stat.h>

using namespace std;

bool file_exists(const string &file_path){
    struct stat buffer;
    return  (stat (file_path.c_str(), &buffer) == 0);
}
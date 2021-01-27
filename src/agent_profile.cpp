#include <agent_profile.h>

using namespace std;


bool Agent_profile::match(double area) {
    return ( area >= lower_bound &&
             (upper_bound == 0 || area <= upper_bound));
}

vector<Agent_profile> Agent_profiles::get_matching_profiles(double area) {
    vector<Agent_profile> matching_profiles;
    for (auto &profile: profiles){
        if (profile.match(area)) matching_profiles.push_back(profile);
    }
    return matching_profiles;
}


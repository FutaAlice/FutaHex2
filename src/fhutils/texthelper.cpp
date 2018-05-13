#include "texthelper.h"

using namespace std;
namespace fhutils {
namespace texthelper {

string trim(const string &src) {
    string cp(src);
    if (cp.empty())
        return cp;
    cp.erase(0, cp.find_first_not_of(" "));
    cp.erase(0, cp.find_first_not_of("\n"));
    cp.erase(0, cp.find_first_not_of("\r"));
    cp.erase(0, cp.find_first_not_of("\n"));
    cp.erase(cp.find_last_not_of(" ") + 1);
    cp.erase(cp.find_last_not_of(" \n") + 1);
    cp.erase(cp.find_last_not_of(" \r") + 1);
    cp.erase(cp.find_last_not_of(" \n") + 1);
    return cp;
}

vector<string>
splitNotEmpty(const string &src, const string &seperator) {
    vector<string> strvec;
    string::size_type pos1, pos2;
    pos2 = src.find(seperator);
    pos1 = 0;
    while (string::npos != pos2) {
        if (!src.substr(pos1, pos2 - pos1).empty())
            strvec.push_back(src.substr(pos1, pos2 - pos1));
        pos1 = pos2 + 1;
        pos2 = src.find(seperator, pos1);
    }
    if (!src.substr(pos1).empty())
        strvec.push_back(src.substr(pos1));
    return strvec;
}

} // namespace texthelper
} // namespace fhutils

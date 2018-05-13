#pragma once
#include <string>
#include <vector>
#include <sstream>

namespace fhutils {
namespace texthelper {

template <typename T>
std::string toString(T t) {
    std::stringstream ss;
    ss << t;
    return ss.str();
}

template <typename T>
T fromString(std::string str) {
    T t;
    std::stringstream ss(str);
    ss >> t;
    return t;
}

std::string trim(const std::string &);

std::vector<std::string>
splitNotEmpty(const std::string &src, const std::string &seperator);

} // namespace texthelper
} // namespace fhutils

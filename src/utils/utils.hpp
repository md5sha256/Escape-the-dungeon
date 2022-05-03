#ifndef UTILS_HPP
#define UTILS_HPP

#include "string"
#include "vector"
#include "sstream"

std::string toString(const std::vector<std::string> &data) {
    std::stringstream ss;
    for (const std::string &s : data) {
        ss << s << " ";
    }
    return ss.str();
}

std::string toLowercase(std::string string) {
    for (char &c : string) {
        c = (char) tolower(c);
    }
    return string;
}

std::string toUppercase(std::string string) {
    for (char &c : string) {
        c = (char) toupper(c);
    }
    return string;
}

#endif//UTILS_HPP

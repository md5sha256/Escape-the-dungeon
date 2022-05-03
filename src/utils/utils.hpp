#ifndef UTILS_HPP
#define UTILS_HPP

#include "string"

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

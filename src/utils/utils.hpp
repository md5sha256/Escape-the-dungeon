#ifndef UTILS_HPP
#define UTILS_HPP

#include "optional.h"
#include "random"
#include "sstream"
#include "string"
#include "vector"

inline std::string toString(const std::vector<std::string> &data) {
    std::stringstream ss;
    for (const std::string &s : data) {
        ss << s << " ";
    }
    return ss.str();
}

inline std::string toLowercase(std::string string) {
    for (char &c : string) {
        c = (char) tolower(c);
    }
    return string;
}

inline std::string toUppercase(std::string string) {
    for (char &c : string) {
        c = (char) toupper(c);
    }
    return string;
}

inline int clamp(const int &min, const int &max, const int &value) noexcept(false) {
    if (min > max) {
        throw std::invalid_argument("Min (" + std::to_string(min) + ") cannot be greater than max (" + std::to_string(max) + ")");
    }
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    }
    return value;
}

inline int clampPercent(const int &value) noexcept(true) {
    return clamp(0, 100, value);
}

inline int clampPositive(const int &value) noexcept(true) {
    if (value < 0) {
        return 0;
    }
    return value;
}

inline int clampNegative(const int &value) noexcept(true) {
    if (value > 0) {
        return 0;
    }
    return value;
}

inline Optional<int> fromString(const std::string &raw) {
    std::stringstream ss(raw);
    int i;
    if (!(ss >> i)) {
        return nullopt<int>();
    }
    return Optional<int>{&i};
}

inline int randInt(const int &min, const int &max) {
    std::default_random_engine engine(rand());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(engine);
}

inline int randIntPercent() {
    return randInt(0, 100);
}

inline bool randBool() {
    return randInt(0, 1) != 0;
}

template<typename T>
inline T randomElement(const std::vector<T> &vec) {
    int index = randInt(0, vec.size());
    return vec[index];
}

#endif//UTILS_HPP

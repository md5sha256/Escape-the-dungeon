#ifndef UTILS_H
#define UTILS_H

#include "optional.h"
#include "random"
#include "sstream"
#include "string"
#include "vector"

/**
 * Useful util to concatenate a vector of strings
 * @param data The strings to concatenate
 * @return Returns a space delimited concatenation of elements in the order of the vector
 */
inline std::string toString(const std::vector<std::string> &data) {
    std::stringstream ss;
    for (const std::string &s : data) {
        ss << s << " ";
    }
    return ss.str();
}

/**
 * Convert a string to lowercase
 * @param string The string to turn lowercase
 * @return Returns a new string which has all lowercase characters
 */
inline std::string toLowercase(std::string string) {
    for (char &c : string) {
        c = (char) tolower(c);
    }
    return string;
}

/**
 * Convert a string to uppercase
 * @param string The string to turn uppercase
 * @return Returns a new string which has all uppercase characters
 */
inline std::string toUppercase(std::string string) {
    for (char &c : string) {
        c = (char) toupper(c);
    }
    return string;
}

/**
 * Clamp an int to within a specified range
 * @param min The min value
 * @param max The max value
 * @param value The value to clamp
 * @return Returns the clamped value within the bound of min and max
 * @throws std::invalid_argument if min > max
 */
inline int clamp(const int &min, const int &max, const int &value) noexcept(false) {
    // Validate min max relation
    if (min > max) {
        throw std::invalid_argument("Min (" + std::to_string(min) + ") cannot be greater than max (" + std::to_string(max) + ")");
    }
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    }
    // Value does not need clamping
    return value;
}

/**
 * Clamp a value to range of a percentage, from 0 - 100.
 * <br>
 * This is the same as calling
 * <code>clamp(0, 100, value)</code>
 * @param value The value to clamp
 * @return The clamped value
 */
inline int clampPercent(const int &value) noexcept(true) {
    return clamp(0, 100, value);
}

/**
 * Clamp a value to be positive
 * @param value The value to clamp
 * @return The clamped value
 */
inline int clampPositive(const int &value) noexcept(true) {
    if (value < 0) {
        return 0;
    }
    return value;
}

/**
 * Clamp a value to be negative
 * @param value The value to clamp
 * @return The clamped value
 */
inline int clampNegative(const int &value) noexcept(true) {
    if (value > 0) {
        return 0;
    }
    return value;
}

/**
 * Parse an integer from a string
 * If the string is invalid and cannot be parsed the
 * resulting <code>Optional</code> will be empty
 * @param raw The string to parse
 * @return An <code>Optional</code> int representing the parsed value
 */
inline Optional<int> fromString(const std::string &raw) {
    std::stringstream ss(raw);
    int i;
    if (!(ss >> i)) {
        return nullopt<int>();
    }
    return Optional<int>{i};
}

/**
 * Generate a random integer within a given bound
 * @param min The min value
 * @param max The max value
 * @return Returns an int between min and max
 */
inline int randInt(const int &min, const int &max) {
    std::default_random_engine engine(rand());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(engine);
}

/**
 * Generate a random percentage
 * @return Returns a int between 0 and 100
 */
inline int randIntPercent() {
    return randInt(0, 100);
}

/**
 * Generate a random boolean
 * @return
 */
inline bool randBool() {
    return randInt(0, 1) != 0;
}

/**
 * Select a random element from a vector
 * @tparam T The type of elements in the vector
 * @param vec The vector to select from
 * @return Returns a random element from this vector
 */
template<typename T>
inline T randomElement(const std::vector<T> &vec) {
    int index = randInt(0, vec.size());
    return vec[index];
}

#endif//UTILS_H

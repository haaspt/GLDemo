//
// Created by Patrick Haas on 11/11/25.
//

#pragma once

#include <glm/glm.hpp>
#include <cmath>

template <typename T> int sign(T value) {
    if (value > T(0)) return 1;
    if (value < T(0)) return -1;
    return 0;
}

inline double magnitude(const glm::vec2& vec) {
    return std::sqrt(
        (vec.x * vec.x) + (vec.y * vec.y)
        );
}

inline double magnitude(const glm::vec3& vec) {
    return std::sqrt(
        (vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z)
        );
}

template <typename T> T clamp(T value, T min, T max) {
    value = std::max(value, min);
    value = std::min(value, max);
    return value;
}

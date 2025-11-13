//
// Created by Patrick Haas on 11/11/25.
//

#pragma once

#include <filesystem>
#include <string>
#include <algorithm>

namespace utils
{
constexpr double PI = 3.14159265358979323846;
constexpr double DEG_TO_RAD = PI / 180.0;
constexpr double RAD_TO_DEG = 180.0 / PI;

class IdGen {
private:
    inline static unsigned int current_id = 0;
public:
    static unsigned int get_id() { return current_id++; }
};

template <typename T> inline int sign(T value) {
    if (value > T(0)) return 1;
    if (value < T(0)) return -1;
    return 0;
}

template <typename T> inline T clamp(T value, T min, T max) {
    value = std::max(value, min);
    value = std::min(value, max);
    return value;
}

inline std::filesystem::path exe_dir_path_from_argv0(const char* argv0) {
    std::filesystem::path exe = std::filesystem::absolute(std::filesystem::path(argv0));
    return exe.parent_path();
}

inline std::filesystem::path shader_path_from_argv0(const char* argv0, const std::string& filename) {
    std::filesystem::path exe = std::filesystem::absolute(std::filesystem::path(argv0));
    std::filesystem::path dir = exe.parent_path() / "shaders";
    return dir / filename;
}
inline double to_degrees(double rad) {
    return rad * RAD_TO_DEG;
}

inline double to_radians(double deg) {
    return deg * DEG_TO_RAD;
}
} // namespace utils



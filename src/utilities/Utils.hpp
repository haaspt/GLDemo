//
// Created by Patrick Haas on 11/11/25.
//

#pragma once

#include <filesystem>
#include <string>
#include <algorithm>
#include <random>
#include <cmath>


namespace Utils {
    constexpr double PI = 3.14159265358979323846;
    constexpr double DEG_TO_RAD = PI / 180.0;
    constexpr double RAD_TO_DEG = 180.0 / PI;

    class IdGen {
    private:
        inline static unsigned int current_id = 0;

    public:
        static unsigned int get_id() { return current_id++; }
    };

    class CircularBuffer {
    private:
        std::vector<float> data;
        size_t index = 0;
        bool filled = false;

    public:
        explicit CircularBuffer(size_t size) : data(size, 0.0f) {
        }

        void push(float value) {
            data[index] = value;
            index = (index + 1) % data.size();
            if (index == 0) filled = true;
        }

        float average_value(size_t last_n = 5) const {
            size_t actual_size = filled ? data.size() : index;
            if (actual_size == 0) return 0.0f;

            last_n = std::min(last_n, actual_size);

            float sum = 0.0f;
            for (size_t i = 0; i < last_n; i++) {
                size_t idx = (index - 1 - i + data.size()) % data.size();
                sum += data[idx];
            }

            return sum / static_cast<float>(last_n);
        }

        float min_value(size_t last_n = 5) const {
            size_t actual_size = filled ? data.size() : index;
            if (actual_size == 0) return 0.0f;

            last_n = std::min(last_n, actual_size);

            float min = data[index];
            for (size_t i = 1; i < last_n; i++) {
                size_t idx = (index - 1 - i + data.size()) % data.size();
                min = std::min(min, data[idx]);
            }
            return min;
        }

        float* buffer() { return data.data(); }
        size_t size() const { return filled ? data.size() : index; }
    };

    template<typename T>
    inline int sign(T value) {
        if (value > T(0)) return 1;
        if (value < T(0)) return -1;
        return 0;
    }

    template<typename T>
    inline T clamp(T value, T min, T max) {
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

    inline double wrap_radians(double rad) {
        double wrapped = std::fmod(rad, PI * 2);
        return wrapped < 0.0 ? wrapped + (PI * 2) : wrapped;
    }

    class Random {
    public:
        static float range(float min, float max) {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_real_distribution<float> dist(min, max);
            return dist(gen);
        }

        static int range(int min, int max) {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dist(min, max);
            return dist(gen);
        }
    };
} // namespace utils
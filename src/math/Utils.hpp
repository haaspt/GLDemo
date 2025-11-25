//
// Created by Tyler Haas on 11/25/25.
//

#pragma once

#include <cmath>

namespace Math {
constexpr double EPSILON = 1e-6;

    constexpr bool almost_equal(double x, double y) {
        return std::fabs(x - y) < EPSILON;
    }
}
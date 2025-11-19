//
// Created by Patrick Haas on 11/15/25.
//

#pragma once

#include "objects/GameObject.hpp"
#include "utilities/Vector.hpp"

class Cube : public GameObject {
public:
    Cube() : GameObject("cube", "default") {}

    const double max_warp_speed = 1000;
    const double max_lateral_speed = 450;

    double warp_accel = 400;
    double lateral_accel = 400;
    double lateral_damping = 800;

    double warp_speed = 600;

    Vector3 accel_vector{0.0};

    void process(double const /*delta_t*/) override {};

};
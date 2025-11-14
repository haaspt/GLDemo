//
// Created by Patrick Haas on 11/14/25.
//

#pragma once

#include <objects/GameObject.hpp>
#include <utilities/Vector.hpp>
#include <utilities/Input.hpp>
#include <utilities/Utils.hpp>
#include <game/Globals.hpp>

class Pyramid : public GameObject {
public:
    Pyramid() : GameObject("pyramid", "default") {}

    const double max_warp_speed = 1000;
    const double max_lateral_speed = 450;

    double warp_accel = 400;
    double lateral_accel = 400;
    double lateral_damping = 800;

    double warp_speed = 600;

    Vector3 accel_vector{0.0};

    void process(double const delta_t) override;

};
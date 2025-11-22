//
// Created by Patrick Haas on 11/15/25.
//

#pragma once

#include "objects/GameObject.hpp"
#include "utilities/Vector.hpp"

class Cube : public GameObject {
public:
    Cube() : GameObject("cube", "default", "default.jpg") {
    }

    void process(double const delta_t) override { rotation_rad.y += delta_t; }
};

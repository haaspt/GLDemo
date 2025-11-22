//
// Created by Patrick Haas on 11/14/25.
//

#pragma once

#include "objects/GameObject.hpp"
#include "utilities/Vector.hpp"
#include "utilities/Input.hpp"
#include "utilities/Utils.hpp"

#include "game/Globals.hpp"

class LightSource : public GameObject {
public:
    LightSource() : GameObject("cube", "light_source") {}

    void process(double const delta_t) override {
        Vector3 input_vec = Input::get_input_vec();
        position.x += input_vec.x * delta_t * -1.0;
        position.y += input_vec.y * delta_t * 1.0;
        position.z += input_vec.z * delta_t * 1.0;
    }

};
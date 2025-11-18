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
    LightSource() : GameObject("pyramid", "light_source") {}

    void process(double const /*delta_t*/) override {}

};
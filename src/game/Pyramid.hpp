//
// Created by Patrick Haas on 11/14/25.
//

#pragma once

#include "objects/GameObject.hpp"

class Pyramid : public GameObject {
public:
    Pyramid() : GameObject("pyramid", "default") {}
    
    void process(double const delta_t) override;

};
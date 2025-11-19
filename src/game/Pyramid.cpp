//
// Created by Patrick Haas on 11/14/25.
//

#include "Pyramid.hpp"

void Pyramid::process(double const delta_t) {
    rotation_rad.y += delta_t;
}

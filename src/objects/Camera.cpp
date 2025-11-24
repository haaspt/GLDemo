//
// Created by Patrick Haas on 11/23/25.
//

#include "Camera.hpp"

void Camera::update(double delta_t) {
    if (controller) {
        controller->update(*this, delta_t);
    }
}

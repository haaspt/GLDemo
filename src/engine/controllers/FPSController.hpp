//
// Created by Patrick Haas on 11/24/25.
//

#pragma once
#include "engine/controllers/BaseController.hpp"
#include "engine/utilities/Utils.hpp"


class FPSController : public BaseController {
private:
    double move_speed;
    double look_speed;

    double pitch = 0.0;
    double yaw = 0.0;

public:
    FPSController(double move_speed, double look_speed)
        : move_speed(move_speed),
          look_speed(Utils::to_radians(look_speed)) {
    }

    void update(Node &node, double delta_t) override;
};

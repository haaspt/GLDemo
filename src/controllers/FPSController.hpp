//
// Created by Patrick Haas on 11/24/25.
//

#pragma once
#include "controllers/BaseController.hpp"
#include "utilities/Utils.hpp"


class FPSController : public BaseController{
private:
    double move_speed;
    double look_speed;
public:
    FPSController(double move_speed, double look_speed) : move_speed(move_speed), look_speed(Utils::to_radians(look_speed)) {}
    void update(Node& node, double delta_t) override;
};

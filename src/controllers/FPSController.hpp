//
// Created by Patrick Haas on 11/24/25.
//

#pragma once
#include "controllers/BaseController.hpp"


class FPSController : public BaseController{
public:
    void update(Node& node, double delta_t) override;
};

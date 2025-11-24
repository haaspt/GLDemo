//
// Created by Patrick Haas on 11/23/25.
//

#pragma once

class Node;

class BaseController {
public:
    virtual ~BaseController() = default;

private:
    virtual void update(Node& node, double delta_t) = 0;
};
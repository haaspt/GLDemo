//
// Created by Tyler Haas on 12/1/25.
//

#pragma once
#include "BaseController.hpp"
#include "objects/Node.hpp"

class FollowController : public BaseController {
private:
    Vector3 offset_ = Vector3(0.0);
    Node* anchor_node_ = nullptr;
public:
    FollowController(Vector3 offset=Vector3(0.0), Node* anchor_node=nullptr)
        : offset_(offset), anchor_node_(anchor_node) {}

    void set_anchor(Node* anchor_ptr) { anchor_node_ = anchor_ptr; }
    void set_offset(Vector3 offset) { offset_ = offset; }

    Node* get_anchor() const { return anchor_node_; }
    Vector3 get_offset() const { return offset_; }

    void update(Node& node, double delta_t) override;
};

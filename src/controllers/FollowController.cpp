//
// Created by Tyler Haas on 12/1/25.
//

#include "FollowController.hpp"

void FollowController::update(Node& node, double delta_t) {
    if (anchor_node_) {
        Vector3 anchor_pos = anchor_node_->get_global_position();
        Vector3 target_pos = anchor_pos + offset_;
        Vector3 node_global_pos = node.get_global_position();
        Vector3 delta_pos = target_pos - node_global_pos ;
        node.set_position(node.get_position() + delta_pos);
    }
}

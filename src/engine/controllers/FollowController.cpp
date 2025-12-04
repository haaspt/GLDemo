//
// Created by Tyler Haas on 12/1/25.
//

#include "FollowController.hpp"

void FollowController::update(Node& node, double delta_t) {
    if (anchor_node_) {
        Transform anchor_trans = anchor_node_->get_global_transform();
        anchor_trans.translate(offset_);
        node.set_position(anchor_trans.get_translation());
        node.set_rotation(anchor_trans.get_rotation());
    }
}

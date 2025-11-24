//
// Created by Patrick Haas on 11/24/25.
//

#include "FPSController.hpp"

#include "objects/Node.hpp"
#include "utilities/Input.hpp"
#include "utilities/Vector.hpp"

void FPSController::update(Node& node, double delta_t) {
    Vector2 cursor_move_vec = Input::get_cursor_vec();
    Vector3 rot_vec{0};
    rot_vec.x = cursor_move_vec.y * look_speed;
    rot_vec.y = cursor_move_vec.x * look_speed;

    double max_speed = .4;

    rot_vec.x = Utils::clamp(rot_vec.x, -max_speed, max_speed);
    rot_vec.y = Utils::clamp(rot_vec.y, -max_speed, max_speed);

    node.rotate_rad(rot_vec.x, rot_vec.y, 0.0);

    const Transform& trans = node.get_transform();

    Vector3 right = Vector3(trans.at(0, 0), trans.at(0, 1), trans.at(0, 2)).normalized();
    Vector3 up = Vector3(trans.at(1, 0), trans.at(1, 1), trans.at(1, 2)).normalized();
    Vector3 forward = Vector3(trans.at(2, 0), trans.at(2, 1), trans.at(2, 2)).normalized();

    Vector3 input_vec = Input::get_input_vec();
    double strafe = -input_vec.x;
    double lift = -input_vec.z;
    double fwd = input_vec.y;

    Vector3 local_vec = Vector3{strafe, lift, fwd}.normalized();

    Vector3 delta_pos = (right * local_vec.x + up * local_vec.y + forward*local_vec.z) * move_speed * delta_t;
    node.set_position(node.get_position() + delta_pos);
}

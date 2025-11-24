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

    Vector3 forward{trans.at(2,0), trans.at(2, 1), trans.at(2, 2)};
    int z_move_dir = Input::get_input_vec().y;
    Vector3 lateral{trans.at(0,0), trans.at(0, 1), trans.at(0, 2)};
    int x_move_dir = -Input::get_input_vec().x;
    Vector3 current_pos = node.get_position();
    Vector3 new_position = current_pos + ((forward * z_move_dir + lateral * x_move_dir) * delta_t * move_speed);
    node.set_position(new_position);

}

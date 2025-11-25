//
// Created by Patrick Haas on 11/24/25.
//

#include "FPSController.hpp"

#include "objects/Node.hpp"
#include "utilities/Input.hpp"
#include "math/Vector.hpp"

void FPSController::update(Node& node, double delta_t) {
    // Mouse look
    Vector2 cursor_move_vec = Input::get_cursor_vec();

    yaw += -cursor_move_vec.x * look_speed;
    pitch += cursor_move_vec.y * look_speed;

    constexpr double max_pitch = Utils::to_radians(89.0);
    pitch = Utils::clamp(pitch, -max_pitch, max_pitch);

    node.set_rotation_rad({pitch, yaw, 0.0});

    // Movement
    const Transform &trans = node.get_transform();

    // Forward basis from transform column
    Vector3 forward = Vector3(trans.at(0, 2), trans.at(1, 2), trans.at(2, 2)).normalized();

    // Global up
    const Vector3 world_up{0.0, 1.0, 0.0};

    // Reproject forward onto global y plane
    // If we don't reproject we can "climb up" by panning up and going forward/back
    Vector3 forward_flat = forward - world_up * forward.dot(world_up);
    forward_flat.normalize();

    // Create perpendicular right
    Vector3 right_flat = forward_flat.cross(world_up).normalized();

    Vector3 input_vec = Input::get_input_vec();

    double strafe = input_vec.x;
    double lift = input_vec.z;
    double fwd = input_vec.y;

    Vector3 local_vec{strafe, lift, fwd};
    local_vec.normalize();

    Vector3 delta_pos = (right_flat * local_vec.x + world_up * local_vec.y + forward_flat * local_vec.z) * move_speed *
                        delta_t;

    node.set_position(node.get_position() + delta_pos);
}

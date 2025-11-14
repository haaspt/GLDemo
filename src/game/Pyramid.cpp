//
// Created by Patrick Haas on 11/14/25.
//

#include "Pyramid.hpp"

void Pyramid::process(double const delta_t) {
    accel_vector = Input::get_input_vec();
    accel_vector.x *= lateral_accel;
    accel_vector.y *= lateral_accel;
    accel_vector.z *= warp_accel;

    velocity += accel_vector * delta_t;

    if (std::abs(accel_vector.x) == 0) {  // No X, apply damping
        velocity.x += lateral_damping * delta_t * -Utils::sign(velocity.x);
        if (std::abs(velocity.x) < 1.0) {
            velocity.x = 0.0f;
        }
    }
    if (std::abs(accel_vector.y) == 0) {  // No X, apply damping
        velocity.y += lateral_damping * delta_t * -Utils::sign(velocity.y);
        if (std::abs(velocity.y) < 1.0) {
            velocity.y = 0.0f;
        }
    }

    velocity.x = Utils::clamp(velocity.x, -max_lateral_speed, max_lateral_speed);
    velocity.y = Utils::clamp(velocity.y, -max_lateral_speed, max_lateral_speed);
    velocity.z = Utils::clamp(velocity.z, 0.0, max_warp_speed);

    if (position.z > 0.1) {
        set_position(
            Vector3(
                Utils::Random::range(-Globals::WORLD_BOUND_X, Globals::WORLD_BOUND_X),
                Utils::Random::range(-Globals::WORLD_BOUND_Y, Globals::WORLD_BOUND_Y),
                -500.0
            )
        );
    }

    // X/Y wrap around
    if (position.x > Globals::WORLD_BOUND_X) {
        position.x = -Globals::WORLD_BOUND_X;
    } else if (position.x < -Globals::WORLD_BOUND_X) {
        position.x = Globals::WORLD_BOUND_X;
    }

    if (position.y > Globals::WORLD_BOUND_Y) {
        position.y = -Globals::WORLD_BOUND_Y;
    } else if (position.y < -Globals::WORLD_BOUND_Y) {
        position.y = Globals::WORLD_BOUND_Y;
    }

    Vector3 rot{1.0};
    rot *= (60.0 * delta_t);
    rotate_deg(rot);
}

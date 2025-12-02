//
// Created by Patrick Haas on 11/23/25.
//

#include "Camera.hpp"
#include "math/Transform.hpp"

void Camera::update(double delta_t) {
    if (controller_) {
        controller_->update(*this, delta_t);
    }
}

Transform Camera::get_view_matrix() const {
    // Camera world position
    Vector3 eye = get_global_position();

    // Use a normalized copy of the rotation
    Quaternion q = get_global_transform().get_rotation();

    // Local helper to rotate a vector by quaternion: v' = q * v * q^{-1}
    auto rotate_vec = [&](const Vector3& v) -> Vector3 {
        Quaternion p{v.x, v.y, v.z, 0.0};   // pure quaternion
        Quaternion inv = q.get_inverse();    // assumes q is normalized
        Quaternion r = q * p * inv;
        return {r.x, r.y, r.z};
    };

    const Vector3 global_forward{0.0, 0.0, -1.0};
    const Vector3 global_up     {0.0, 1.0, 0.0};

    Vector3 forward = rotate_vec(global_forward);
    Vector3 up      = rotate_vec(global_up);

    return look_at(eye, eye + forward, up);
}

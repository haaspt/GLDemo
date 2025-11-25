//
// Created by Patrick Haas on 11/24/25.
//

#include "Quaternion.hpp"
#include "math/Transform.hpp"

Transform Quaternion::to_transform()  {
    if (!is_normalized()) {
        normalize();
    }
    return Transform{*this};
}

Vector3 Quaternion::to_euler() {
    if (!is_normalized()) {
        normalize();
    }
    Vector3 euler;

    // Roll (x-axis rotation)
    double sinr_cosp = 2 * (w * x + y * z);
    double cosr_cosp = 1 - 2 * (x * x + y * y);
    euler.x = std::atan2(sinr_cosp, cosr_cosp);

    // Pitch (y-axis rotation)
    double sinp = 2 * (w * y - z * x);
    if (std::abs(sinp) >= 1)
        euler.y = std::copysign(M_PI / 2, sinp); // Use 90 degrees if out of range
    else
        euler.y = std::asin(sinp);

    // Yaw (z-axis rotation)
    double siny_cosp = 2 * (w * z + x * y);
    double cosy_cosp = 1 - 2 * (y * y + z * z);
    euler.z = std::atan2(siny_cosp, cosy_cosp);

    return euler;
}

Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs) {
    return {
        lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x,
        lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w,
        lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z
    };
}

Quaternion& Quaternion::operator*=(const Quaternion& rhs) {
    double new_x = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
    double new_y = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x;
    double new_z = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w;
    double new_w = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;

    x = new_x;
    y = new_y;
    z = new_z;
    w = new_w;

    is_normalized_ = false;  // Multiplication can denormalize

    return *this;
}
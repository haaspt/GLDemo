//
// Created by Patrick Haas on 11/24/25.
//

#pragma once

#include <cmath>

#include "Transform.hpp"
#include "math/Vector.hpp"

struct Transform;

class Quaternion {
    friend struct Transform;
private:
    bool is_normalized_ = true;

    double x;
    double y;
    double z;
    double w;

public:


    Quaternion() : x(0), y(0), z(0), w(1) {}
    Quaternion(double x, double y, double z, double w)
        : x(x), y(y), z(z), w(w) {
        normalize();
    }
    Quaternion(const Vector3& euler) {
        double half_pitch = euler.x / 2;
        double half_yaw = euler.y / 2;
        double half_roll = euler.z / 2;

        double cp = cos(half_pitch);
        double sp = sin(half_pitch);
        double cy = cos(half_yaw);
        double sy = sin(half_yaw);
        double cr = cos(half_roll);
        double sr = sin(half_roll);

        w = cr * cp * cy + sr * sp * sy;
        x = sr * cp * cy - cr * sp * sy;
        y = cr * sp * cy + sr * cp * sy;
        z = cr * cp * sy - sr * sp * cy;

        normalize();
    }

    friend Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs);
    Quaternion& operator*=(const Quaternion& rhs);

    bool is_normalized() const { return is_normalized_ ; }
    const Quaternion& normalize() {
        double mag = std::sqrt(x*x + y*y + z*z + w*w);
        x = x / mag;
        y = y / mag;
        z = z / mag;
        w = w / mag;

        is_normalized_ = true;

        return *this;
    }
    Quaternion normalized() const {
        // Normalization handled by constructor
        return {x, y, z, w};
    }

    Transform to_transform();
    Vector3 to_euler();

    Quaternion get_inverse() const {
        return {-x, -y, -z, w};
    }
};

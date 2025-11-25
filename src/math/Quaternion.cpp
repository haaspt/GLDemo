//
// Created by Patrick Haas on 11/24/25.
//

#include "Quaternion.hpp"
#include "math/Transform.hpp"
#include "math/Utils.hpp"

Quaternion Quaternion::from_euler(const Vector3& euler) {
    double pitch = euler.x; // X
    double yaw   = euler.y; // Y
    double roll  = euler.z; // Z

    double hp = pitch * 0.5;
    double hy = yaw   * 0.5;
    double hr = roll  * 0.5;

    double sp = std::sin(hp);
    double cp = std::cos(hp);
    double sy = std::sin(hy);
    double cy = std::cos(hy);
    double sr = std::sin(hr);
    double cr = std::cos(hr);

    // quaternions for each axis
    Quaternion q_pitch{  sp, 0.0, 0.0,  cp}; // X
    Quaternion q_yaw  {0.0,  sy, 0.0,  cy}; // Y
    Quaternion q_roll {0.0, 0.0,  sr,  cr}; // Z

    // total rotation: first pitch, then yaw, then roll
    return q_roll * q_yaw * q_pitch;
}

Transform Quaternion::to_transform() const {
    return Transform{*this};
}

Vector3 Quaternion::to_euler() const {
    Quaternion q = this->normalized();

    // We'll compute pitch (around X), yaw (around Y), roll (around Z)
    // for the same R = Rz(roll) * Ry(yaw) * Rx(pitch) convention.

    // sin(pitch) = 2(w*x + y*z)
    double sinp = 2.0 * (q.w * q.x - q.y * q.z);
    double pitch;
    if (std::abs(sinp) >= 1.0) {
        pitch = std::copysign(M_PI / 2.0, sinp); // clamp
    } else {
        pitch = std::asin(sinp);
    }

    // yaw (Y) and roll (Z) from remaining terms
    double siny_cosp = 2.0 * (q.w * q.y + q.z * q.x);
    double cosy_cosp = 1.0 - 2.0 * (q.x * q.x + q.y * q.y);
    double yaw = std::atan2(siny_cosp, cosy_cosp);

    double sinr_cosp = 2.0 * (q.w * q.z + q.x * q.y);
    double cosr_cosp = 1.0 - 2.0 * (q.y * q.y + q.z * q.z);
    double roll = std::atan2(sinr_cosp, cosr_cosp);

    return { pitch, yaw, roll };
}


bool Quaternion::is_normalized() const {
    return Math::almost_equal(length_squared(), 1);
}

const Quaternion &Quaternion::normalize() {
    *this /= length();
    return *this;
}

Quaternion Quaternion::normalized() const {
    return *this / length();
}

Quaternion Quaternion::slerp(const Quaternion &to, double weight) const {
    assert(is_normalized());
    assert(to.is_normalized());

    // Copied from the Godot implementation
    Quaternion to_1;
    double omega, cosom, sinom, scale0, scale1;

    cosom = dot(to);

    if (cosom < 0.0) {
        cosom = -cosom;
        to_1.x = -to.x;
        to_1.y = -to.y;
        to_1.z = -to.z;
        to_1.w = -to.w;
    } else {
        to_1 = to;
    }

    if (!Math::almost_equal(1, cosom)) {
        // standard case (slerp)
        omega = std::acos(cosom);
        sinom = std::sin(omega);
        scale0 = std::sin((1.0 - weight) * omega) / sinom;
        scale1 = std::sin(weight * omega) / sinom;
    } else {
        // "from" and "to" quaternions are very close
        //  ... so we can do a linear interpolation
        scale0 = 1.0f - weight;
        scale1 = weight;
    }

    return Quaternion(
        scale0 * x + scale1 * to_1.x,
        scale0 * y + scale1 * to_1.y,
        scale0 * z + scale1 * to_1.z,
        scale0 * w + scale1 * to_1.w);
}


Quaternion operator*(const Quaternion &lhs, const Quaternion &rhs) {
    return {
        lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x,
        lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w,
        lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z
    };
}

Quaternion operator*(const Quaternion &lhs, double rhs) {
    return Quaternion(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
}


Quaternion operator/(const Quaternion &lhs, double rhs) {
    return lhs * (1 / rhs);
}


Quaternion &Quaternion::operator*=(const Quaternion &rhs) {
    double xx = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
    double yy = w * rhs.y + y * rhs.w + z * rhs.x - x * rhs.z;
    double zz = w * rhs.z + z * rhs.w + x * rhs.y - y * rhs.x;
    w = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
    x = xx;
    y = yy;
    z = zz;

    return *this;
}

Quaternion &Quaternion::operator*=(double rhs) {
    x *= rhs;
    y *= rhs;
    z *= rhs;
    w *= rhs;

    return *this;
}


Quaternion &Quaternion::operator/=(double rhs) {
    return *this *= (1 / rhs);
}

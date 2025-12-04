//
// Created by Patrick Haas on 11/24/25.
//

#pragma once

#include <cmath>

#include "engine/math/Transform.hpp"
#include "engine/math/Vector.hpp"

struct Quaternion {
    double x;
    double y;
    double z;
    double w;

    Quaternion() : x(0), y(0), z(0), w(1) {
    }

    Quaternion(double x, double y, double z, double w)
        : x(x), y(y), z(z), w(w) {
    }

    static Quaternion from_euler(const Vector3 &euler);

    Transform to_transform() const;

    Vector3 to_euler() const;

    bool is_normalized() const;

    const Quaternion &normalize();

    Quaternion normalized() const;

    Quaternion get_inverse() const {
        assert(is_normalized());
        return {-x, -y, -z, w};
    }

    double dot(const Quaternion &other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    Quaternion slerp(const Quaternion &to, double weight) const;

    double length_squared() const {
        return dot(*this);
    }

    double length() const {
        return std::sqrt(length_squared());
    }

    friend Quaternion operator*(const Quaternion &lhs, const Quaternion &rhs);

    friend Quaternion operator*(const Quaternion &lhs, double rhs);

    friend Quaternion operator/(const Quaternion &lhs, double rhs);

    friend bool operator==(const Quaternion &lhs, const Quaternion &rhs);

    friend bool operator!=(const Quaternion &lhs, const Quaternion &rhs);

    Quaternion &operator*=(const Quaternion &rhs);

    Quaternion &operator*=(double rhs);

    Quaternion &operator/=(double rhs);
};

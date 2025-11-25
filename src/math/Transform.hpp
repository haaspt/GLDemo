//
// Created by Patrick Haas on 11/24/25.
//

#pragma once

#include <assimp/matrix4x4.h>
#include <glm/glm.hpp>

#include "math/Vector.hpp"

struct Quaternion;


struct Transform {
private:
    double data[16];

public:
    Transform() : Transform(1.0) {}

    explicit Transform(double scalar) : data{0} {
        // x 0 0 0
        // 0 x 0 0
        // 0 0 x 0
        // 0 0 0 x
        data[0]  = scalar;
        data[5]  = scalar;
        data[10] = scalar;
        data[15] = scalar;
    }

    explicit Transform(const aiMatrix4x4& ai_matrix4x4) : data{0} {
        data[0]  = ai_matrix4x4.a1;
        data[1]  = ai_matrix4x4.a2;
        data[2]  = ai_matrix4x4.a3;
        data[3]  = ai_matrix4x4.a4;
        data[4]  = ai_matrix4x4.b1;
        data[5]  = ai_matrix4x4.b2;
        data[6]  = ai_matrix4x4.b3;
        data[7]  = ai_matrix4x4.b4;
        data[8]  = ai_matrix4x4.c1;
        data[9]  = ai_matrix4x4.c2;
        data[10] = ai_matrix4x4.c3;
        data[11] = ai_matrix4x4.c4;
        data[12] = ai_matrix4x4.d1;
        data[13] = ai_matrix4x4.d2;
        data[14] = ai_matrix4x4.d3;
        data[15] = ai_matrix4x4.d4;
    }

    explicit Transform(const Quaternion& quat);

    double& operator[](size_t index) { return data[index]; }
    const double& operator[](size_t index) const { return data[index]; }

    double& at(size_t row, size_t col) {
        return data[row * 4 + col];
    }

    const double& at(size_t row, size_t col) const {
        return data[row * 4 + col];
    }

    friend Transform operator*(const Transform& lhs, const Transform& rhs);

    Transform& operator*=(const Transform& rhs);

    Transform& translate(const Vector3& translation_vec);

    Transform& scale(const Vector3& scale_vec);

    Transform& rotate(double radians, const Vector3& axis);

    static Transform perspective(double fov_rad, double aspect, double z_near, double z_far);

    glm::mat4 to_glm() const {
        return {
            // column 0
            static_cast<float>(data[0]),  static_cast<float>(data[4]),
            static_cast<float>(data[8]),  static_cast<float>(data[12]),
            // column 1
            static_cast<float>(data[1]),  static_cast<float>(data[5]),
            static_cast<float>(data[9]),  static_cast<float>(data[13]),
            // column 2
            static_cast<float>(data[2]),  static_cast<float>(data[6]),
            static_cast<float>(data[10]), static_cast<float>(data[14]),
            // column 3
            static_cast<float>(data[3]),  static_cast<float>(data[7]),
            static_cast<float>(data[11]), static_cast<float>(data[15])
        };
    }
};

Transform look_at(const Vector3& eye, const Vector3& target, const Vector3& up = Vector3(0.0, 1.0, 0.0));

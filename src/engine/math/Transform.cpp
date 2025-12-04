//
// Created by Patrick Haas on 11/24/25.
//

#include "engine/math/Transform.hpp"
#include "engine/math/Quaternion.hpp"

Transform::Transform(const Quaternion& quat)  : data{0} {
    double d = quat.length_squared();
    double s = 2.0 / d;

    double xs = quat.x * s, ys = quat.y * s, zs = quat.z * s;
    double wx = quat.w * xs, wy = quat.w * ys, wz = quat.w * zs;
    double xx = quat.x * xs, xy = quat.x * ys, xz = quat.x * zs;
    double yy = quat.y * ys, yz = quat.y * zs, zz = quat.z * zs;

    data[0]  = 1.0 - (yy + zz);
    data[1]  =        xy - wz;
    data[2]  =        xz + wy;
    data[3]  = 0.0;

    data[4]  =        xy + wz;
    data[5]  = 1.0 - (xx + zz);
    data[6]  =        yz - wx;
    data[7]  = 0.0;

    data[8]  =        xz - wy;
    data[9]  =        yz + wx;
    data[10] = 1.0 - (xx + yy);
    data[11] = 0.0;

    data[12] = 0.0;
    data[13] = 0.0;
    data[14] = 0.0;
    data[15] = 1.0;
}


Transform operator*(const Transform& lhs, const Transform& rhs) {
    Transform out;

    for (int r = 0; r < 4; r++) {
        const int r0 = r * 4;
        for (int c = 0; c < 4; c++) {
            out[r0 + c] = lhs.data[r0 + 0] * rhs.data[0 * 4 + c] +
                          lhs.data[r0 + 1] * rhs.data[1 * 4 + c] +
                          lhs.data[r0 + 2] * rhs.data[2 * 4 + c] +
                          lhs.data[r0 + 3] * rhs.data[3 * 4 + c];
        }
    }

    return out;
}

Transform& Transform::operator*=(const Transform& rhs) {
    // temp array
    double out[16];

    for (int r = 0; r < 4; ++r) {
        const int r0 = r * 4;
        for (int c = 0; c < 4; ++c) {
            out[r0 + c] =
                    data[r0 + 0] * rhs.data[0 * 4 + c] +
                    data[r0 + 1] * rhs.data[1 * 4 + c] +
                    data[r0 + 2] * rhs.data[2 * 4 + c] +
                    data[r0 + 3] * rhs.data[3 * 4 + c];
        }
    }

    // copy back
    for (int i = 0; i < 16; ++i) data[i] = out[i];
    return *this;
}

Transform& Transform::translate(const Vector3& vec) {
    // 1 0 0 delta_x
    // 0 1 0 delta_y
    // 0 0 1 delta_z
    // 0 0 0 1
    Transform t(1.0);
    t.at(0, 3) = vec.x;
    t.at(1, 3) = vec.y;
    t.at(2, 3) = vec.z;

    *this *= t;
    return *this;
}

Transform& Transform::scale(const Vector3& vec) {
    // scale column 0 (X basis)
    data[0]  *= vec.x;
    data[4]  *= vec.x;
    data[8]  *= vec.x;
    data[12] *= vec.x;

    // scale column 1 (Y basis)
    data[1]  *= vec.y;
    data[5]  *= vec.y;
    data[9]  *= vec.y;
    data[13] *= vec.y;

    // scale column 2 (Z basis)
    data[2]  *= vec.z;
    data[6]  *= vec.z;
    data[10] *= vec.z;
    data[14] *= vec.z;

    return *this;
}

Transform& Transform::rotate(double radians, const Vector3& axis) {
    // Normalize axis
    double axis_mag = axis.magnitude();
    double x = axis.x / axis_mag;
    double y = axis.y / axis_mag;
    double z = axis.z / axis_mag;

    double c = std::cos(radians);
    double s = std::sin(radians);
    double ic = 1.0 - c;

    // Rotation matix
    // [ xx*ic + c     xy*ic - z*s   xz*ic + y*s   0 ]
    // [ yx*ic + z*s   yy*ic + c     yz*ic - x*s   0 ]
    // [ zx*ic - y*s   zy*ic + x*s   zz*ic + c     0 ]
    // [ 0             0             0             1 ]
    Transform r(1.0);

    r[0]  = x*x*ic + c;     r[1]  = x*y*ic - z*s;  r[2]  = x*z*ic + y*s;  r[3]  = 0.0;
    r[4]  = y*x*ic + z*s;   r[5]  = y*y*ic + c;    r[6]  = y*z*ic - x*s;  r[7]  = 0.0;
    r[8]  = z*x*ic - y*s;   r[9]  = z*y*ic + x*s;  r[10] = z*z*ic + c;    r[11] = 0.0;
    r[12] = 0.0;            r[13] = 0.0;           r[14] = 0.0;           r[15] = 1.0;

    // Apply rotation
    *this *= r;
    return *this;
}

Vector3 Transform::get_translation() const {
    return {at(0, 3), at(1, 3), at(2, 3)};
}

Vector3 Transform::get_scale() const {
    double scale_x = Vector3(data[0], data[4], data[8]).magnitude();
    double scale_y = Vector3(data[1], data[5], data[9]).magnitude();
    double scale_z = Vector3(data[2], data[6], data[10]).magnitude();
    return {scale_x, scale_y, scale_z};
}

Quaternion Transform::get_rotation() const {
    Vector3 s = get_scale();

    // Guard for div-by-zero
    double inv_sx = (s.x != 0.0) ? 1.0 / s.x : 0.0;
    double inv_sy = (s.y != 0.0) ? 1.0 / s.y : 0.0;
    double inv_sz = (s.z != 0.0) ? 1.0 / s.z : 0.0;

    double m00 = data[0]  * inv_sx;
    double m01 = data[1]  * inv_sy;
    double m02 = data[2]  * inv_sz;

    double m10 = data[4]  * inv_sx;
    double m11 = data[5]  * inv_sy;
    double m12 = data[6]  * inv_sz;

    double m20 = data[8]  * inv_sx;
    double m21 = data[9]  * inv_sy;
    double m22 = data[10] * inv_sz;

    Quaternion q;
    double trace = m00 + m11 + m22;

    if (trace > 0.0) {
        double t = std::sqrt(trace + 1.0);
        q.w = 0.5 * t;
        double inv_t = 0.5 / t;
        q.x = (m21 - m12) * inv_t;
        q.y = (m02 - m20) * inv_t;
        q.z = (m10 - m01) * inv_t;
    } else if (m00 >= m11 && m00 >= m22) {
        double t = std::sqrt(1.0 + m00 - m11 - m22);
        double inv_t = 0.5 / t;
        q.x = 0.5 * t;
        q.y = (m01 + m10) * inv_t;
        q.z = (m02 + m20) * inv_t;
        q.w = (m21 - m12) * inv_t;
    } else if (m11 > m22) {
        double t = std::sqrt(1.0 + m11 - m00 - m22);
        double inv_t = 0.5 / t;
        q.x = (m01 + m10) * inv_t;
        q.y = 0.5 * t;
        q.z = (m12 + m21) * inv_t;
        q.w = (m02 - m20) * inv_t;
    } else {
        double t = std::sqrt(1.0 + m22 - m00 - m11);
        double inv_t = 0.5 / t;
        q.x = (m02 + m20) * inv_t;
        q.y = (m12 + m21) * inv_t;
        q.z = 0.5 * t;
        q.w = (m10 - m01) * inv_t;
    }

    // 4. Normalize (for numerical robustness)
    return q.normalized();
}

Vector3 Transform::get_forward() const {
    // 3rd column represents z axis
    Vector3 z_axis = {data[2], data[6], data[10]};

    return (z_axis * -1).normalize();
}

Vector3 Transform::get_up() const {
    Vector3 y_axis = {data[1], data[5], data[9]};

    return y_axis.normalize();
}

Vector3 Transform::get_right() const {
    Vector3 x_axis = {data[0], data[4], data[8]};

    return x_axis.normalize();
}


Transform Transform::perspective(double fov_rad, double aspect, double z_near, double z_far) {
    // [f/aspect, 0, 0, 0]
    // [0, f, 0, 0]
    // [0, 0, -(f+n)/(f-n), -2fn/(f-n)]
    // [0, 0, -1, 0]
    const double tan_half_fov = std::tan(fov_rad / 2.0);
    const double f = 1.0 / tan_half_fov;

    Transform result(0.0);
    result.at(0, 0) = f / aspect;
    result.at(1, 1) = f;
    result.at(2, 2) = -(z_far + z_near) / (z_far - z_near);
    result.at(2, 3) = -(2.0 * z_far * z_near) / (z_far - z_near);
    result.at(3, 2) = -1.0;
    result.at(3, 3) = 0.0;
    return result;
}

Transform look_at(const Vector3& eye, const Vector3& target, const Vector3& up) {
    const Vector3 f = (target - eye).normalized();
    const Vector3 s = f.cross(up).normalized();
    const Vector3 u = s.cross(f);

    Transform result(1.0);

    // [R.x, R.y, R.z, -R·eye]
    // [U.x, U.y, U.z, -U·eye]
    // [-F.x, -F.y, -F.z, F·eye]
    // [0, 0, 0, 1]

    result.at(0, 0) = s.x;
    result.at(0, 1) = s.y;
    result.at(0, 2) = s.z;
    result.at(0, 3) = -s.dot(eye);

    result.at(1, 0) = u.x;
    result.at(1, 1) = u.y;
    result.at(1, 2) = u.z;
    result.at(1, 3) = -u.dot(eye);

    result.at(2, 0) = -f.x;
    result.at(2, 1) = -f.y;
    result.at(2, 2) = -f.z;
    result.at(2, 3) = f.dot(eye);

    return result;
}

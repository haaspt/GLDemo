//
// Created by Patrick Haas on 11/24/25.
//

#include "Transform.hpp"
#include "math/Quaternion.hpp"

Transform::Transform(const Quaternion& quat)  : data{0} {
    double xx = quat.x * quat.x;
    double yy = quat.y * quat.y;
    double zz = quat.z * quat.z;
    double xy = quat.x * quat.y;
    double xz = quat.x * quat.z;
    double yz = quat.y * quat.z;
    double wx = quat.w * quat.x;
    double wy = quat.w * quat.y;
    double wz = quat.w * quat.z;

    data[0]  = 1 - 2*(yy + zz);
    data[1]  = 2*(xy - wz);
    data[2]  = 2*(xz + wy);
    data[3]  = 0;

    data[4]  = 2*(xy + wz);
    data[5]  = 1 - 2*(xx + zz);
    data[6]  = 2*(yz - wx);
    data[7]  = 0;

    data[8]  = 2*(xz - wy);
    data[9]  = 2*(yz + wx);
    data[10] = 1 - 2*(xx + yy);
    data[11] = 0;

    data[12] = 0;
    data[13] = 0;
    data[14] = 0;
    data[15] = 1;
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

//
// Created by Patrick Haas on 11/11/25.
//

#pragma once

#include <cmath>

#include <glm/glm.hpp>
#include <assimp/matrix4x4.h>
#include <assimp/vector3.h>

#include "utilities/Utils.hpp"


struct Vector2 {
    double x;
    double y;

    Vector2() : Vector2(0.0) {}
    Vector2(double x, double y) : x(x), y(y) {}
    explicit Vector2(double scalar) : x(scalar), y(scalar) {}
    static Vector2 ZERO() {return Vector2(0);}
    static Vector2 UP() {return Vector2(0, -1);}
    static Vector2 DOWN() {return Vector2(0, 1);}
    static Vector2 LEFT() {return Vector2(-1, 0);}
    static Vector2 RIGHT() {return Vector2(1, 0);}

    Vector2& operator+=(const Vector2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    friend Vector2 operator+(const Vector2& rhs, const Vector2& lhs) {
        return Vector2(rhs.x + lhs.x, rhs.y + lhs.y);
    }

    Vector2& operator-=(const Vector2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    friend Vector2 operator-(const Vector2& rhs, const Vector2& lhs) {
        return Vector2(rhs.x - lhs.x, rhs.y - lhs.y);
    }

    Vector2& operator*=(const Vector2& other) {
        x *= other.x;
        y *= other.y;
        return *this;
    }
    Vector2& operator*=(double scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    friend Vector2 operator*(const Vector2& lhs, const Vector2& rhs) {
        return Vector2(rhs.x * lhs.x, rhs.y * lhs.y);
    }
    friend Vector2 operator*(const Vector2& vec, double scalar) {
        return Vector2(vec.x * scalar, vec.y * scalar);
    }
    friend Vector2 operator*(double scalar, const Vector2& vec) {
        return vec * scalar;
    }

    Vector2& operator/=(const Vector2& other) {
        x /= other.x;
        y /= other.y;
        return *this;
    }
    Vector2& operator/=(double other) {
        x /= other;
        y /= other;
        return *this;
    }
    friend Vector2 operator/(const Vector2& lhs, const Vector2& rhs) {
        return Vector2(lhs.x / rhs.x, lhs.y / rhs.y);
    }
    friend Vector2 operator/(const Vector2& vec, double scalar) {
        return Vector2(vec.x / scalar, vec.y / scalar);
    }

    bool operator==(const Vector2& other) const {
        return (x == other.x) and (y == other.y);
    }
    bool operator!=(const Vector2& other) const {
        return !(*this == other);
    }

    double magnitude() const {return std::sqrt((x * x) + (y * y));}
    void normalize() {
        double mag = magnitude();
        x /= mag;
        y /= mag;
    }
    Vector2 normalized() const {
        Vector2 result;
        
        double mag = magnitude();
        result.x = x / mag;
        result.y = y / mag;
        
        return result;
    }

    double cross_z(const Vector2& other) const {
        return x * other.y - y * other.x;
    }

    glm::vec2 to_glm() const {return {static_cast<float>(x), static_cast<float>(y)};};

    Vector2 to_radians() const {
        return Vector2(Utils::to_radians(x), Utils::to_radians(y));
    }
    Vector2 to_degrees() const {
        return Vector2(Utils::to_degrees(x), Utils::to_degrees(y));
    }
};

struct Vector3 {
    double x;
    double y;
    double z;

    Vector3() : Vector3(0.0) {}
    Vector3(double x, double y, double z): x(x), y(y), z(z) {}
    explicit Vector3(double scalar): x(scalar), y(scalar), z(scalar) {}
    explicit Vector3(const aiVector3D& ai_vector3) : x(ai_vector3.x), y(ai_vector3.y), z(ai_vector3.z) {}
    static Vector3 ZERO() {return Vector3(0);}

    Vector3& operator+=(const Vector3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    friend Vector3 operator+(const Vector3& lhs, const Vector3& rhs) {
        return Vector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
    }

    Vector3& operator-=(const Vector3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    friend Vector3 operator-(const Vector3& lhs, const Vector3& rhs) {
        return Vector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
    }

    Vector3& operator*=(const Vector3& other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }
    Vector3& operator*=(double scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    friend Vector3 operator*(const Vector3& lhs, const Vector3& rhs) {
        return Vector3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
    }
    friend Vector3 operator*(const Vector3& vec, double scalar) {
        return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
    }
    friend Vector3 operator*(double scalar, const Vector3& vec) {
        return vec * scalar;
    }

    Vector3& operator/=(const Vector3& other) {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }
    Vector3& operator/=(double other) {
        x /= other;
        y /= other;
        z /= other;
        return *this;
    }
    friend Vector3 operator/(const Vector3& lhs, const Vector3& rhs) {
        return Vector3(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
    }
    friend Vector3 operator/(const Vector3& vec, double scalar) {
        return Vector3(vec.x / scalar, vec.y / scalar, vec.z / scalar);
    }

    bool operator==(const Vector3& other) const {
        return (x == other.x) and (y == other.y) and (z == other.z);
    }
    bool operator!=(const Vector3& other) const {
        return !(*this == other);
    }

    double magnitude() const {return std::sqrt((x * x) + (y * y) + (z * z));}
    void normalize() {
        double mag = magnitude();
        x /= mag;
        y /= mag;
        z /= mag;
    }
    Vector3 normalized() const {
        Vector3 result;
        
        double mag = magnitude();
        result.x = x / mag;
        result.y = y / mag;
        result.z = z / mag;

        return result;
    }

    Vector3 cross(const Vector3& other) const;
    friend Vector3 cross(const Vector3& a, const Vector3& b) {
        return Vector3(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        );
    }

    double dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    friend double dot(const Vector3& a, const Vector3& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    glm::vec3 to_glm() const {return {static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)};}
    Vector3 to_radians() const {
        return Vector3(Utils::to_radians(x), Utils::to_radians(y), Utils::to_radians(z));
    }
    Vector3 to_degrees() const {
        return Vector3(Utils::to_degrees(x), Utils::to_degrees(y), Utils::to_degrees(z));
    }
};

struct Vector4 {
    double r;
    double g;
    double b;
    double a;

    Vector4() : Vector4(0.0) {}
    Vector4(double r, double g, double b, double a) : r(r), g(g), b(b), a(a) {}
    explicit Vector4(double scalar) : r(scalar), g(scalar), b(scalar), a(scalar) {}
    static Vector4 ZERO() {return Vector4(0);}

    Vector4& operator+=(const Vector4& other) {
        r += other.r;
        g += other.g;
        b += other.b;
        a += other.a;
        return *this;
    }
    friend Vector4 operator+(const Vector4& lhs, const Vector4& rhs) {
        return Vector4(lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b, lhs.a + rhs.a);
    }

    Vector4& operator-=(const Vector4& other) {
        r -= other.r;
        g -= other.g;
        b -= other.b;
        a -= other.a;
        return *this;
    }
    friend Vector4 operator-(const Vector4& lhs, const Vector4& rhs) {
        return Vector4(lhs.r - rhs.r, lhs.g - rhs.g, lhs.b - rhs.b, lhs.a - rhs.a);
    }

    Vector4& operator*=(const Vector4& other) {
        r *= other.r;
        g *= other.g;
        b *= other.b;
        a *= other.a;
        return *this;
    }
    Vector4& operator*=(double scalar) {
        r *= scalar;
        g *= scalar;
        b *= scalar;
        a *= scalar;
        return *this;
    }
    friend Vector4 operator*(const Vector4& lhs, const Vector4& rhs) {
        return Vector4(lhs.r * rhs.r, lhs.g * rhs.g, lhs.b * rhs.b, lhs.a * rhs.a);
    }
    friend Vector4 operator*(const Vector4& vec, double scalar) {
        return Vector4(vec.r * scalar, vec.g * scalar, vec.b * scalar, vec.a * scalar);
    }
    friend Vector4 operator*(double scalar, const Vector4& vec) {
        return vec * scalar;
    }

    Vector4& operator/=(const Vector4& other) {
        r /= other.r;
        g /= other.g;
        b /= other.b;
        a /= other.a;
        return *this;
    }
    Vector4& operator/=(double other) {
        r /= other;
        g /= other;
        b /= other;
        a /= other;
        return *this;
    }
    friend Vector4 operator/(const Vector4& lhs, const Vector4& rhs) {
        return Vector4(lhs.r / rhs.r, lhs.g / rhs.g, lhs.b / rhs.b, lhs.a / rhs.a);
    }
    friend Vector4 operator/(const Vector4& vec, double scalar) {
        return Vector4(vec.r / scalar, vec.g / scalar, vec.b / scalar, vec.a / scalar);
    }

    bool operator==(const Vector4& other) const {
        return (r == other.r) and (g == other.g) and (b == other.b) and (a == other.a);
    }
    bool operator!=(const Vector4& other) const {
        return !(*this == other);
    }

    double magnitude() const {return std::sqrt((r * r) + (g * g) + (b * b) + (a * a));}
    void normalize() {
        double mag = magnitude();
        r /= mag;
        g /= mag;
        b /= mag;
        a /= mag;
    }
    Vector4 normalized() const {
        Vector4 result;

        double mag = magnitude();
        result.r = r / mag;
        result.g = g / mag;
        result.b = b / mag;
        result.a = a / mag;

        return result;
    }

    glm::vec4 to_glm() const {return {static_cast<float>(r), static_cast<float>(g), static_cast<float>(b), static_cast<float>(a)};}
    Vector4 to_radians() const {
        return Vector4(Utils::to_radians(r), Utils::to_radians(g), Utils::to_radians(b), Utils::to_radians(a));
    }
    Vector4 to_degrees() const {
        return Vector4(Utils::to_degrees(r), Utils::to_degrees(g), Utils::to_degrees(b), Utils::to_degrees(a));
    }
};

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

    double& operator[](size_t index) {return data[index];}
    const double& operator[](size_t index) const {return data[index];}
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
        return glm::mat4(
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
        );
    }
};

Transform look_at(const Vector3& eye, const Vector3& target, const Vector3& up = Vector3(0.0, 1.0, 0.0));
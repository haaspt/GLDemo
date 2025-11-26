//
// Created by Tyler Haas on 11/25/25.
//
#include <gtest/gtest.h>
#include "../src/math/Quaternion.hpp"
#include "../src/math/Vector.hpp"
#include <glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>
#include <cmath>

static constexpr double EPS = 1e-9;

TEST(QuaternionTest, Identity) {
    Quaternion q;
    glm::quat glm_quat = glm::identity<glm::quat>();

    EXPECT_DOUBLE_EQ(q.x, 0.0);
    EXPECT_DOUBLE_EQ(q.y, 0.0);
    EXPECT_DOUBLE_EQ(q.z, 0.0);
    EXPECT_DOUBLE_EQ(q.w, 1.0);

    EXPECT_DOUBLE_EQ(q.x, static_cast<double>(glm_quat.x));
    EXPECT_DOUBLE_EQ(q.y, static_cast<double>(glm_quat.y));
    EXPECT_DOUBLE_EQ(q.z, static_cast<double>(glm_quat.z));
    EXPECT_DOUBLE_EQ(q.w, static_cast<double>(glm_quat.w));
}

TEST(QuaternionTest, Product) {
    Quaternion p(1.0, -2.0, 1.0, 3.0);
    Quaternion q(-1.0, 2.0, 3.0, 2.0);

    Quaternion pq = p * q;
    EXPECT_DOUBLE_EQ(pq.x, -9.0);
    EXPECT_DOUBLE_EQ(pq.y, -2.0);
    EXPECT_DOUBLE_EQ(pq.z, 11.0);
    EXPECT_DOUBLE_EQ(pq.w, 8.0);
}

TEST(QuaternionTest, LengthAndNormalize) {
    Quaternion q(1.0, 2.0, 3.0, 4.0);
    double len_sq = q.length_squared();
    double len    = q.length();

    EXPECT_NEAR(len_sq, 1.0*1.0 + 2.0*2.0 + 3.0*3.0 + 4.0*4.0, EPS);
    EXPECT_NEAR(len, std::sqrt(len_sq), EPS);

    Quaternion n = q.normalized();
    EXPECT_NEAR(n.length(), 1.0, 1e-12);
    EXPECT_NEAR(n.length_squared(), 1.0, 1e-12);
}

TEST(QuaternionTest, NormalizeInPlace) {
    Quaternion q(0.5, -0.5, 0.5, -0.5);
    q.normalize();
    EXPECT_NEAR(q.length(), 1.0, 1e-12);
}

TEST(QuaternionTest, InverseIsIdentityWhenMultiplied) {
    // 90 degrees around X axis
    double angle = M_PI / 2.0;
    double s = std::sin(angle / 2.0);
    double c = std::cos(angle / 2.0);
    Quaternion q(s, 0.0, 0.0, c);  // axis (1,0,0)
    q.normalize();

    Quaternion inv = q.get_inverse();
    Quaternion id = q * inv;

    EXPECT_NEAR(id.x, 0.0, 1e-12);
    EXPECT_NEAR(id.y, 0.0, 1e-12);
    EXPECT_NEAR(id.z, 0.0, 1e-12);
    EXPECT_NEAR(id.w, 1.0, 1e-12);
}

// --- Euler sanity tests (NOT full round-trips) ---

TEST(QuaternionTest, FromEuler_PurePitch) {
    // Pitch about X only
    double pitch = 0.5; // radians
    Vector3 euler(pitch, 0.0, 0.0);

    Quaternion q = Quaternion::from_euler(euler);
    q.normalize();

    // A pure X rotation of angle θ should look like:
    // q = (sin(θ/2), 0, 0, cos(θ/2)) in our convention
    double s = std::sin(pitch * 0.5);
    double c = std::cos(pitch * 0.5);

    EXPECT_NEAR(q.x, s, 1e-6);
    EXPECT_NEAR(q.y, 0.0, 1e-6);
    EXPECT_NEAR(q.z, 0.0, 1e-6);
    EXPECT_NEAR(q.w, c, 1e-6);
}

TEST(QuaternionTest, FromEuler_PureYaw) {
    // Yaw about Y only
    double yaw = -0.7;
    Vector3 euler(0.0, yaw, 0.0);

    Quaternion q = Quaternion::from_euler(euler);
    q.normalize();

    // A pure Y rotation of angle θ should look like:
    // q = (0, sin(θ/2), 0, cos(θ/2))
    double s = std::sin(yaw * 0.5);
    double c = std::cos(yaw * 0.5);

    EXPECT_NEAR(q.x, 0.0, 1e-6);
    EXPECT_NEAR(q.y, s, 1e-6);
    EXPECT_NEAR(q.z, 0.0, 1e-6);
    EXPECT_NEAR(q.w, c, 1e-6);
}

TEST(QuaternionTest, FromEuler_PureRoll) {
    // Roll about Z only
    double roll = 1.2;
    Vector3 euler(0.0, 0.0, roll);

    Quaternion q = Quaternion::from_euler(euler);
    q.normalize();

    // A pure Z rotation of angle θ should look like:
    // q = (0, 0, sin(θ/2), cos(θ/2))
    double s = std::sin(roll * 0.5);
    double c = std::cos(roll * 0.5);

    EXPECT_NEAR(q.x, 0.0, 1e-6);
    EXPECT_NEAR(q.y, 0.0, 1e-6);
    EXPECT_NEAR(q.z, s, 1e-6);
    EXPECT_NEAR(q.w, c, 1e-6);
}

// Optional: very simple to_euler check at the identity
TEST(QuaternionTest, ToEuler_IdentityIsZeroAngles) {
    Quaternion q; // identity
    Vector3 e = q.to_euler();
    EXPECT_NEAR(e.x, 0.0, 1e-6);
    EXPECT_NEAR(e.y, 0.0, 1e-6);
    EXPECT_NEAR(e.z, 0.0, 1e-6);
}

// --- SLERP basic properties ---

TEST(QuaternionTest, SlerpEndpoints) {
    // identity
    Quaternion q0; // (0,0,0,1)

    // 180 degrees around Z axis
    double angle = M_PI;
    double s = std::sin(angle / 2.0);
    double c = std::cos(angle / 2.0);
    Quaternion q1(0.0, 0.0, s, c);
    q1.normalize();

    // t = 0 -> q0
    Quaternion a = q0.slerp(q1, 0.0);
    EXPECT_NEAR(a.x, q0.x, 1e-12);
    EXPECT_NEAR(a.y, q0.y, 1e-12);
    EXPECT_NEAR(a.z, q0.z, 1e-12);
    EXPECT_NEAR(a.w, q0.w, 1e-12);

    // t = 1 -> q1
    Quaternion b = q0.slerp(q1, 1.0);
    EXPECT_NEAR(b.x, q1.x, 1e-12);
    EXPECT_NEAR(b.y, q1.y, 1e-12);
    EXPECT_NEAR(b.z, q1.z, 1e-12);
    EXPECT_NEAR(b.w, q1.w, 1e-12);

    // t = 0.5: still normalized
    Quaternion mid = q0.slerp(q1, 0.5);
    EXPECT_NEAR(mid.length(), 1.0, 1e-12);
}

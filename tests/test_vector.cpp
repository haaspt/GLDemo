#include <gtest/gtest.h>
#include "utilities/Vector.hpp"
#include <cmath>

// Test Vector2 basic operations
TEST(Vector2Test, DefaultConstructor) {
    Vector2 v;
    EXPECT_DOUBLE_EQ(v.x, 0.0);
    EXPECT_DOUBLE_EQ(v.y, 0.0);
}

TEST(Vector2Test, ScalarConstructor) {
    Vector2 v(5.0);
    EXPECT_DOUBLE_EQ(v.x, 5.0);
    EXPECT_DOUBLE_EQ(v.y, 5.0);
}

TEST(Vector2Test, ComponentConstructor) {
    Vector2 v(1.0, 2.0);
    EXPECT_DOUBLE_EQ(v.x, 1.0);
    EXPECT_DOUBLE_EQ(v.y, 2.0);
}

TEST(Vector2Test, Addition) {
    Vector2 a(1.0, 2.0);
    Vector2 b(3.0, 4.0);
    Vector2 c = a + b;
    EXPECT_DOUBLE_EQ(c.x, 4.0);
    EXPECT_DOUBLE_EQ(c.y, 6.0);
}

TEST(Vector2Test, Subtraction) {
    Vector2 a(5.0, 7.0);
    Vector2 b(2.0, 3.0);
    Vector2 c = a - b;
    EXPECT_DOUBLE_EQ(c.x, 3.0);
    EXPECT_DOUBLE_EQ(c.y, 4.0);
}

TEST(Vector2Test, ScalarMultiplication) {
    Vector2 v(2.0, 3.0);
    Vector2 result = v * 2.0;
    EXPECT_DOUBLE_EQ(result.x, 4.0);
    EXPECT_DOUBLE_EQ(result.y, 6.0);
}

TEST(Vector2Test, Magnitude) {
    Vector2 v(3.0, 4.0);
    EXPECT_DOUBLE_EQ(v.magnitude(), 5.0);
}

TEST(Vector2Test, Normalize) {
    Vector2 v(3.0, 4.0);
    v.normalize();
    EXPECT_NEAR(v.magnitude(), 1.0, 1e-10);
}

// Test Vector3 basic operations
TEST(Vector3Test, DefaultConstructor) {
    Vector3 v;
    EXPECT_DOUBLE_EQ(v.x, 0.0);
    EXPECT_DOUBLE_EQ(v.y, 0.0);
    EXPECT_DOUBLE_EQ(v.z, 0.0);
}

TEST(Vector3Test, ComponentConstructor) {
    Vector3 v(1.0, 2.0, 3.0);
    EXPECT_DOUBLE_EQ(v.x, 1.0);
    EXPECT_DOUBLE_EQ(v.y, 2.0);
    EXPECT_DOUBLE_EQ(v.z, 3.0);
}

TEST(Vector3Test, Addition) {
    Vector3 a(1.0, 2.0, 3.0);
    Vector3 b(4.0, 5.0, 6.0);
    Vector3 c = a + b;
    EXPECT_DOUBLE_EQ(c.x, 5.0);
    EXPECT_DOUBLE_EQ(c.y, 7.0);
    EXPECT_DOUBLE_EQ(c.z, 9.0);
}

TEST(Vector3Test, DotProduct) {
    Vector3 a(1.0, 2.0, 3.0);
    Vector3 b(4.0, 5.0, 6.0);
    double dot = a.dot(b);
    EXPECT_DOUBLE_EQ(dot, 32.0); // 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
}

TEST(Vector3Test, CrossProduct) {
    Vector3 a(1.0, 0.0, 0.0);
    Vector3 b(0.0, 1.0, 0.0);
    Vector3 c = a.cross(b);
    EXPECT_DOUBLE_EQ(c.x, 0.0);
    EXPECT_DOUBLE_EQ(c.y, 0.0);
    EXPECT_DOUBLE_EQ(c.z, 1.0);
}

TEST(Vector3Test, CrossProductReverse) {
    Vector3 a(1.0, 0.0, 0.0);
    Vector3 b(0.0, 1.0, 0.0);
    Vector3 c = b.cross(a);
    EXPECT_DOUBLE_EQ(c.x, 0.0);
    EXPECT_DOUBLE_EQ(c.y, 0.0);
    EXPECT_DOUBLE_EQ(c.z, -1.0);
}

TEST(Vector3Test, Magnitude) {
    Vector3 v(2.0, 3.0, 6.0);
    EXPECT_DOUBLE_EQ(v.magnitude(), 7.0); // sqrt(4 + 9 + 36) = sqrt(49) = 7
}

TEST(Vector3Test, Normalize) {
    Vector3 v(3.0, 4.0, 0.0);
    v.normalize();
    EXPECT_NEAR(v.magnitude(), 1.0, 1e-10);
    EXPECT_DOUBLE_EQ(v.x, 0.6);
    EXPECT_DOUBLE_EQ(v.y, 0.8);
    EXPECT_DOUBLE_EQ(v.z, 0.0);
}

TEST(Vector3Test, ToGLM) {
    Vector3 v(1.0, 2.0, 3.0);
    glm::vec3 glm_v = v.to_glm();
    EXPECT_FLOAT_EQ(glm_v.x, 1.0f);
    EXPECT_FLOAT_EQ(glm_v.y, 2.0f);
    EXPECT_FLOAT_EQ(glm_v.z, 3.0f);
}


//
// Created by Tyler Haas on 11/25/25.
//
#include <gtest/gtest.h>
#include "../src/math/Quaternion.hpp"
#include <glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>

TEST(QuaternionTest, Identity) {
    Quaternion q;
    glm::quat glm_quat = glm::identity<glm::quat>();
    EXPECT_TRUE(q.x == q.y == q.z == 0.0);
    EXPECT_TRUE(q.w == 1.0);
    EXPECT_TRUE(q.x == glm_quat.x);
    EXPECT_TRUE(q.y == glm_quat.y);
    EXPECT_TRUE(q.z == glm_quat.z);
    EXPECT_TRUE(q.w == glm_quat.w);
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
#include <gtest/gtest.h>
#include <cmath>

#include "../src/objects/Node.hpp"
#include "../src/math/Vector.hpp"
#include "../src/math/Transform.hpp"
#include "../src/math/Quaternion.hpp"

// Simple subclass to verify process() is called by update()
class TestNode : public Node {
public:
    mutable int process_count = 0;

protected:
    void process(double /*delta_t*/) override {
        ++process_count;
    }
};

constexpr double NODE_EPS = 1e-6;

static bool matricesEqual(const glm::mat4& a, const glm::mat4& b, double eps = NODE_EPS) {
    for (int i = 0; i < 4; ++i) {      // column
        for (int j = 0; j < 4; ++j) {  // row
            if (std::abs(a[i][j] - b[i][j]) > eps) {
                return false;
            }
        }
    }
    return true;
}

// Default node should have identity transform
TEST(NodeTest, DefaultTransformIsIdentity) {
    Node n;
    const Transform& t = n.get_transform();

    glm::mat4 m = t.to_glm();
    glm::mat4 identity(1.0f);

    EXPECT_TRUE(matricesEqual(m, identity));
}

// Setting position should show up as translation in the transform
TEST(NodeTest, PositionAffectsTransformTranslation) {
    Node n;
    n.set_position(1.0, 2.0, 3.0);

    const Transform& t = n.get_transform();
    glm::mat4 m = t.to_glm();

    // Translation is column 3 in glm::mat4 (x,y,z,w)
    EXPECT_NEAR(m[3][0], 1.0, NODE_EPS);
    EXPECT_NEAR(m[3][1], 2.0, NODE_EPS);
    EXPECT_NEAR(m[3][2], 3.0, NODE_EPS);
    EXPECT_NEAR(m[3][3], 1.0, NODE_EPS);
}

// Setting scale should affect the transform (relative to identity)
TEST(NodeTest, ScaleAffectsTransform) {
    Node n;
    n.set_scale(2.0, 3.0, 4.0);

    const Transform& t = n.get_transform();
    glm::mat4 m = t.to_glm();

    // Diagonal entries of upper-left 3x3 should be scale factors,
    // since rotation is identity here.
    EXPECT_NEAR(m[0][0], 2.0, NODE_EPS);
    EXPECT_NEAR(m[1][1], 3.0, NODE_EPS);
    EXPECT_NEAR(m[2][2], 4.0, NODE_EPS);
}

// Rotation via set_rotation_rad should match an equivalent Transform composition
TEST(NodeTest, RotationMatchesTransformFromQuaternion) {
    Node n;

    Vector3 pos(1.0, 2.0, 3.0);
    Vector3 scale(2.0, 3.0, 4.0);
    Vector3 euler(0.3, -0.7, 1.0); // pitch, yaw, roll (your convention)

    n.set_position(pos);
    n.set_scale(scale);
    n.set_rotation_rad(euler);

    const Transform& node_t = n.get_transform();
    glm::mat4 node_m = node_t.to_glm();

    // Build expected transform using the same building blocks as Node:
    // T * R * S, with R coming from Quaternion::from_euler
    Quaternion q = Quaternion::from_euler(euler).normalized();

    Transform expected(1.0);
    expected.translate(pos);
    expected *= Transform(q);
    expected.scale(scale);

    glm::mat4 expected_m = expected.to_glm();

    EXPECT_TRUE(matricesEqual(node_m, expected_m, 1e-5));
}

// update() should move position by velocity * delta_t
TEST(NodeTest, UpdateAdvancesPositionByVelocity) {
    Node n;

    n.set_position(0.0, 0.0, 0.0);
    n.set_velocity(2.0, -1.0, 0.5);

    double dt = 0.5; // seconds
    n.update(dt);

    Vector3 pos = n.get_position();
    EXPECT_NEAR(pos.x, 2.0 * dt, NODE_EPS);
    EXPECT_NEAR(pos.y, -1.0 * dt, NODE_EPS);
    EXPECT_NEAR(pos.z, 0.5 * dt, NODE_EPS);

    // Transform should reflect new position
    const Transform& t = n.get_transform();
    glm::mat4 m = t.to_glm();
    EXPECT_NEAR(m[3][0], pos.x, NODE_EPS);
    EXPECT_NEAR(m[3][1], pos.y, NODE_EPS);
    EXPECT_NEAR(m[3][2], pos.z, NODE_EPS);
}

// update() should call process()
TEST(NodeTest, UpdateCallsProcess) {
    TestNode n;

    EXPECT_EQ(n.process_count, 0);
    n.update(0.1);
    EXPECT_EQ(n.process_count, 1);

    n.update(0.2);
    EXPECT_EQ(n.process_count, 2);
}

// rotate_rad should change the transform's rotation part
TEST(NodeTest, RotateRadChangesOrientation) {
    Node n;

    // Start with identity
    glm::mat4 before = n.get_transform().to_glm();

    // Apply a small rotation around Y
    n.rotate_rad(0.0, 0.5, 0.0);

    glm::mat4 after = n.get_transform().to_glm();

    // They should not be equal (beyond a tiny epsilon)
    bool same = matricesEqual(before, after, 1e-9);
    EXPECT_FALSE(same);
}

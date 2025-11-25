#include <gtest/gtest.h>
#include "../src/math/Vector.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

const double EPSILON = 1e-6;

// Helper function to compare two matrices
bool matricesEqual(const glm::mat4& a, const glm::mat4& b, double epsilon = EPSILON) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (std::abs(a[i][j] - b[i][j]) > epsilon) {
                return false;
            }
        }
    }
    return true;
}

// Test Transform identity matrix
TEST(TransformTest, Identity) {
    Transform t(1.0);
    glm::mat4 glm_identity = glm::mat4(1.0f);
    glm::mat4 result = t.to_glm();
    
    EXPECT_TRUE(matricesEqual(result, glm_identity));
}

// Test Transform translation
TEST(TransformTest, Translation) {
    Transform t(1.0);
    t.translate(Vector3(5.0, 10.0, 15.0));
    
    glm::mat4 glm_translate = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 10.0f, 15.0f));
    glm::mat4 result = t.to_glm();
    
    EXPECT_TRUE(matricesEqual(result, glm_translate, 1e-5));
}

// Test Transform scale
TEST(TransformTest, Scale) {
    Transform t(1.0);
    t.scale(Vector3(2.0, 3.0, 4.0));
    
    glm::mat4 glm_scale = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 3.0f, 4.0f));
    glm::mat4 result = t.to_glm();
    
    EXPECT_TRUE(matricesEqual(result, glm_scale, 1e-5));
}

// Test Transform rotation around Z axis
TEST(TransformTest, RotationZ) {
    Transform t(1.0);
    double angle = M_PI / 4.0; // 45 degrees
    t.rotate(angle, Vector3(0.0, 0.0, 1.0));
    
    glm::mat4 glm_rotate = glm::rotate(glm::mat4(1.0f), static_cast<float>(angle), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 result = t.to_glm();
    
    EXPECT_TRUE(matricesEqual(result, glm_rotate, 1e-4));
}

// Test Transform rotation around Y axis
TEST(TransformTest, RotationY) {
    Transform t(1.0);
    double angle = M_PI / 6.0; // 30 degrees
    t.rotate(angle, Vector3(0.0, 1.0, 0.0));
    
    glm::mat4 glm_rotate = glm::rotate(glm::mat4(1.0f), static_cast<float>(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 result = t.to_glm();
    
    EXPECT_TRUE(matricesEqual(result, glm_rotate, 1e-4));
}

// Test Transform matrix multiplication
TEST(TransformTest, MatrixMultiplication) {
    Transform a(1.0);
    a.translate(Vector3(1.0, 2.0, 3.0));
    
    Transform b(1.0);
    b.scale(Vector3(2.0, 2.0, 2.0));
    
    Transform c = a * b;
    
    glm::mat4 glm_a = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 3.0f));
    glm::mat4 glm_b = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));
    glm::mat4 glm_c = glm_a * glm_b;
    
    glm::mat4 result = c.to_glm();
    
    EXPECT_TRUE(matricesEqual(result, glm_c, 1e-5));
}

// Test Transform perspective projection
TEST(TransformTest, Perspective) {
    double fov = M_PI / 4.0; // 45 degrees
    double aspect = 16.0 / 9.0;
    double near = 0.1;
    double far = 100.0;
    
    Transform proj = Transform::perspective(fov, aspect, near, far);
    glm::mat4 glm_proj = glm::perspective(static_cast<float>(fov), static_cast<float>(aspect), 
                                          static_cast<float>(near), static_cast<float>(far));
    
    glm::mat4 result = proj.to_glm();
    
    // Print matrices for debugging if they don't match
    if (!matricesEqual(result, glm_proj, 1e-4)) {
        std::cout << "\nCustom perspective matrix:\n";
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                std::cout << result[j][i] << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\nGLM perspective matrix:\n";
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                std::cout << glm_proj[j][i] << " ";
            }
            std::cout << "\n";
        }
    }
    
    EXPECT_TRUE(matricesEqual(result, glm_proj, 1e-4));
}

// Test Transform look_at
TEST(TransformTest, LookAt) {
    Vector3 eye(0.0, 0.0, 5.0);
    Vector3 target(0.0, 0.0, 0.0);
    Vector3 up(0.0, 1.0, 0.0);
    
    Transform view = look_at(eye, target, up);
    glm::mat4 glm_view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 5.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    
    glm::mat4 result = view.to_glm();
    
    EXPECT_TRUE(matricesEqual(result, glm_view, 1e-4));
}

// Test Transform combined operations (translate, rotate, scale)
TEST(TransformTest, CombinedOperations) {
    Transform t(1.0);
    t.translate(Vector3(1.0, 2.0, 3.0));
    t.rotate(M_PI / 4.0, Vector3(0.0, 1.0, 0.0));
    t.scale(Vector3(2.0, 2.0, 2.0));
    
    glm::mat4 glm_t = glm::mat4(1.0f);
    glm_t = glm::translate(glm_t, glm::vec3(1.0f, 2.0f, 3.0f));
    glm_t = glm::rotate(glm_t, static_cast<float>(M_PI / 4.0), glm::vec3(0.0f, 1.0f, 0.0f));
    glm_t = glm::scale(glm_t, glm::vec3(2.0f, 2.0f, 2.0f));
    
    glm::mat4 result = t.to_glm();
    
    EXPECT_TRUE(matricesEqual(result, glm_t, 1e-4));
}


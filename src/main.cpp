#include <vector>
#include <random>
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "GameObject.hpp"
#include "Input.hpp"
#include "Utils.hpp"
#include "Vector.hpp"

constexpr unsigned int W_WIDTH = 800;
constexpr unsigned int W_HEIGHT = 600;

constexpr float WORLD_BOUND_X = 250.0f;
constexpr float WORLD_BOUND_Y = 250.0f;

GLFWwindow* initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "OpenGL Demo", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // TODO set keyboard and resize callbacks

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);
    glfwSetWindowAspectRatio(window, W_WIDTH, W_HEIGHT);

    glEnable(GL_DEPTH_TEST);
    return window;
}

int main() {
    GLFWwindow* window = initWindow();
    Input input(window);

    std::vector<float> verts {
        // position      // RGB
        -0.5, -0.5,  0.5,  0.5, 1.0, 1.0,  // base vertex 0
         0.5, -0.5,  0.5,  1.0, 0.5, 1.0,  // base vertex 1
         0.5, -0.5, -0.5,  1.0, 1.0, 0.5,  // base vertex 2
        -0.5, -0.5, -0.5,  0.5, 1.0, 1.0,  // base vertex 3
         0.0,  0.5,  0.0,  1.0, 1.0, 1.0,  // apex vertex 4
    };
    std::vector<int> idx {
        0, 1, 4,  // front face
        1, 2, 4,  // right face
        2, 3, 4,  // back face
        3, 0, 4,  // left face
        0, 2, 1,  // base triangle 1
        0, 3, 2   // base triangle 2
    };

    Mesh mesh(
        verts, idx
    );

    Shader shader(
        "/Users/tyler.haas/Developer/GLDemo/src/shaders/vertex.glsl",
        "/Users/tyler.haas/Developer/GLDemo/src/shaders/fragment.glsl"
    );

    std::vector<GameObject> entities;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> rand_x(-WORLD_BOUND_X, WORLD_BOUND_X);
    std::uniform_int_distribution<> rand_y(-WORLD_BOUND_Y, WORLD_BOUND_Y);
    std::uniform_int_distribution<> rand_z(-500, 1);

    for (int i=0; i < 500; i++) {
        GameObject obj(&mesh, &shader);
        obj.set_position(
            Vector3(
                rand_x(gen),
                rand_y(gen),
                rand_z(gen)
            )
        );
        entities.push_back(obj);

    }

    Camera camera(55.0, static_cast<double>(W_WIDTH) / W_HEIGHT, 0.1, 500.0);
    camera.set_position(0.0, 0.0, -3.0);
    camera.set_yaw(-90);


    // Movement and physics *should* be done as doubles
    // but since OpenGL/GLM use float precision we'll
    // just use floats for everything
    double delta_t = 0.0;
    double last = glfwGetTime();
    double now = 0.0;

    double max_warp_speed = 1000;
    double max_lateral_speed = 450;

    double warp_accel = 400;
    double lateral_accel = 400;
    double lateral_damping = 800;

    double warp_speed = 600;

    Vector3 velocity_vector{0.0, 0.0, warp_speed};
    Vector3 accel_vector{0.0};
    
    while (!glfwWindowShouldClose(window)) {
        now = glfwGetTime();
        delta_t = now - last;
        last = now;

        accel_vector = input.get_input_vec();

        if (delta_t > 0) {
            accel_vector.x *= lateral_accel;
            accel_vector.y *= lateral_accel;
            accel_vector.z *= warp_accel;

            velocity_vector += accel_vector * delta_t;

            if (std::abs(accel_vector.x) == 0) {  // No X, apply damping
                velocity_vector.x += lateral_damping * delta_t * -utils::sign(velocity_vector.x);
                if (std::abs(velocity_vector.x) < 1.0) {
                    velocity_vector.x = 0.0f;
                }
            }
            if (std::abs(accel_vector.y) == 0) {  // No X, apply damping
                velocity_vector.y += lateral_damping * delta_t * -utils::sign(velocity_vector.y);
                if (std::abs(velocity_vector.y) < 1.0) {
                    velocity_vector.y = 0.0f;
                }
            }

            velocity_vector.x = utils::clamp(velocity_vector.x, -max_lateral_speed, max_lateral_speed);
            velocity_vector.y = utils::clamp(velocity_vector.y, -max_lateral_speed, max_lateral_speed);
            velocity_vector.z = utils::clamp(velocity_vector.z, 0.0, max_warp_speed);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto& object : entities) {
            object.set_velocity(velocity_vector);
            Vector3 pos = object.get_position();

            // Z wrap around
            if (pos.z > 0.1) {
                object.set_position(
                    Vector3(
                        rand_x(gen),
                        rand_y(gen),
                        -500.0
                    )
                );
            }

            // X/Y wrap around
            if (pos.x > WORLD_BOUND_X) {
                pos.x = -WORLD_BOUND_X;
                object.set_position(pos);
            } else if (pos.x < -WORLD_BOUND_X) {
                pos.x = WORLD_BOUND_X;
                object.set_position(pos);
            }

            if (pos.y > WORLD_BOUND_Y) {
                pos.y = -WORLD_BOUND_Y;
                object.set_position(pos);
            } else if (pos.y < -WORLD_BOUND_Y) {
                pos.y = WORLD_BOUND_Y;
                object.set_position(pos);
            }
            Vector3 rot{1.0};
            rot *= (60.0 * delta_t);
            object.rotate_deg(rot);

            object.update(delta_t);
            object.render(camera);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}

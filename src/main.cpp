#include <stdio.h>
#include <vector>
#include <random>
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "GameObject.hpp"

constexpr unsigned int W_WIDTH = 800;
constexpr unsigned int W_HEIGHT = 600;

void initWindow(GLFWwindow*& window) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "OpenGL Demo", NULL, NULL);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // TODO set keyboard and resize callbacks

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);

    glEnable(GL_DEPTH_TEST);
}

int main() {
    GLFWwindow* window = nullptr;
    initWindow(window);

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

    Shader shader(
        "/Users/patrickhaas/Developer/GLDemo/src/shaders/vertex.glsl",
        "/Users/patrickhaas/Developer/GLDemo/src/shaders/fragment.glsl"
    );

    Mesh mesh(
        verts, idx
    );

    std::vector<GameObject> entities;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> rand_x(-250, 250);
    std::uniform_int_distribution<> rand_y(-250, 250);
    std::uniform_int_distribution<> rand_z(-500, 1);

    for (int i=0; i < 500; i++) {
        GameObject obj(&mesh, &shader);
        obj.set_position(
            glm::vec3(
                rand_x(gen),
                rand_y(gen),
                rand_z(gen)
            )
        );
        entities.push_back(obj);

    }

    Camera camera(55.0f, static_cast<float>(W_WIDTH) / W_HEIGHT, 0.1f, 500.0f);
    camera.set_position(0.0f, 0.0f, -3.0f);
    camera.set_yaw(-90);
    
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto& object : entities) {
            glm::vec3 pos = object.get_position();
            object.set_position(
                pos += glm::vec3(0.0f, 0.0f, 5.0f)
            );
            if (pos.z > 0.1) {
                object.set_position(
                    glm::vec3(
                        static_cast<float>(rand_x(gen)),
                         static_cast<float>(rand_y(gen)),
                        -500.0f
                    )
                );
            }
            object.rotate_deg(0.5f, 0.5f, 0.5f);
            object.render(camera);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}

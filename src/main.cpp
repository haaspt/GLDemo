#include <vector>
#include <memory>
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include "objects/Camera.hpp"
#include "objects/GameObject.hpp"
#include "utilities/Utils.hpp"
#include "utilities/Vector.hpp"
#include "resources/ResourceManager.hpp"

#include "game/Globals.hpp"
#include "game/Pyramid.hpp"

constexpr unsigned int W_WIDTH = 800;
constexpr unsigned int W_HEIGHT = 600;


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

int main(int /*argc*/, char** argv) {
    // Window setup
    GLFWwindow* window = initWindow();

    // Singleton setup
    Managers::initialize(Utils::exe_dir_path_from_argv0(argv[0]));
    Input::initialize(window);

    // Camera Setup
    Camera camera(55.0, static_cast<double>(W_WIDTH) / W_HEIGHT, 0.1, 500.0);
    camera.set_position(0.0, 0.0, -3.0);
    camera.set_yaw(-90);

    // World setup
    std::vector<std::unique_ptr<GameObject>> entities;

    for (int i=0; i < 500; i++) {
        auto pyramid = std::make_unique<Pyramid>();
        pyramid->set_position(
            Vector3(
            Utils::Random::range(-Globals::WORLD_BOUND_X, Globals::WORLD_BOUND_X),
            Utils::Random::range(-Globals::WORLD_BOUND_Y, Globals::WORLD_BOUND_Y),
            Utils::Random::range(-500, 1)
            )
        );
        pyramid->set_velocity(0.0, 0.0, 500.0);

        entities.push_back(std::move(pyramid));

    }
    
    double delta_t = 0.0;
    double last = glfwGetTime();
    double now = 0.0;
    
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        now = glfwGetTime();
        delta_t = now - last;
        last = now;

        for (const auto& object : entities) {
            object->update(delta_t);
            object->render(camera);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}

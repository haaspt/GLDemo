#include <vector>
#include <thread>
#include <chrono>

#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>

#include "objects/Camera.hpp"
#include "objects/GameObject.hpp"
#include "objects/LightSource.hpp"
#include "utilities/Input.hpp"
#include "utilities/Vector.hpp"
#include "resources/ResourceManager.hpp"
#include "controllers/FPSController.hpp"


constexpr unsigned int W_WIDTH = 800;
constexpr unsigned int W_HEIGHT = 600;

constexpr double TARGET_FPS = 120;
constexpr double FRAME_DURATION_MS = 1.0 / TARGET_FPS * 1000.0;


GLFWwindow* initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "OpenGL Demo", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    // TODO set keyboard and resize callbacks

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);
    glfwSetWindowAspectRatio(window, W_WIDTH, W_HEIGHT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    return window;
}

int main(int /*argc*/, char** argv) {
    // Window setup
    GLFWwindow* window = initWindow();

    // Singleton setup
    Managers::initialize(Utils::exe_dir_path_from_argv0(argv[0]));
    Input::initialize(window);

    auto suzanne = GameObject("suzanne.gltf", "default");
    suzanne.rotate_deg(0, 0, 0);

    Camera camera(
                65.0,
                static_cast<double>(W_WIDTH) / W_HEIGHT,
                0.1,
                500.0,
                {0, 0, -3.0},
                std::make_unique<FPSController>(5.0, 0.2)
                );


    auto light_source = LightSource("sphere.gltf", Vector3{1.0}, 0.4);
    light_source.set_position(-1.75, 0, 2.75);
    light_source.set_scale(0.5, 0.5, 0.5);


    double delta_t = 0.0;
    double last = glfwGetTime();
    double now = 0.0;
    double this_frame_duration_ms = 0.0;
    double time_to_next_frame_ms = 0.0;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        now = glfwGetTime();
        delta_t = now - last;
        last = now;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();


        Input::poll();


        // Render Loop
        camera.update(delta_t);
        light_source.update(delta_t);
        light_source.render(camera, {});


        suzanne.update(delta_t);
        suzanne.render(camera, {&light_source});

        glfwSwapBuffers(window);

        // Attempt at frame timing
        this_frame_duration_ms = (glfwGetTime() - now) * 1000.0;
        time_to_next_frame_ms = FRAME_DURATION_MS - this_frame_duration_ms;
        if (time_to_next_frame_ms > 0.0) {
            std::this_thread::sleep_for(
                std::chrono::microseconds(static_cast<long>(time_to_next_frame_ms * 1000.0))
            );
        }
    }

    return 0;
}

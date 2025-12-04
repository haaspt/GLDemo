#include "Application.hpp"
#include <OpenGL/gl3.h>
#include <thread>
#include <chrono>

GLFWwindow* Application::init_window() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(window_width_, window_height_, window_name_.c_str(), nullptr, nullptr);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);
    glfwSetWindowAspectRatio(window, window_width_, window_height_);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    return window;
}

void Application::add_prefab_to_scene(const Scene::Prefab& prefab) {
    assert(main_scene_);
    prefab.initialize(*main_scene_);
}

bool Application::loop() {
    assert(initialized);

    double delta_t = 0.0;
    double last = glfwGetTime();
    double now = 0.0;
    double this_frame_duration_ms = 0.0;
    double time_to_next_frame_ms = 0.0;

    while (!glfwWindowShouldClose(window_)) {
        now = glfwGetTime();
        delta_t = now - last;
        last = now;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();

        poll_events();

        process_scene(delta_t);

        glfwSwapBuffers(window_);

        // Attempt at frame timing
        this_frame_duration_ms = (glfwGetTime() - now) * 1000.0;
        time_to_next_frame_ms = FRAME_DURATION_MS - this_frame_duration_ms;
        if (time_to_next_frame_ms > 0.0) {
            std::this_thread::sleep_for(
                std::chrono::microseconds(static_cast<long>(time_to_next_frame_ms * 1000.0))
            );
        }
    }
    return true;
}

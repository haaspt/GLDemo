#include <vector>
#include <memory>
#include <thread>
#include <chrono>

#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "objects/Camera.hpp"
#include "objects/GameObject.hpp"
#include "utilities/Utils.hpp"
#include "utilities/Input.hpp"
#include "utilities/Vector.hpp"
#include "resources/ResourceManager.hpp"

#include "game/Cube.hpp"
#include "game/Pyramid.hpp"
#include "game/LightSource.hpp"

constexpr unsigned int W_WIDTH = 800;
constexpr unsigned int W_HEIGHT = 600;

constexpr double TARGET_FPS = 120;
constexpr double FRAME_DURATION_MS = 1.0 / TARGET_FPS * 1000.0;


GLFWwindow* initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "OpenGL Demo", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    // TODO set keyboard and resize callbacks

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);
    glfwSetWindowAspectRatio(window, W_WIDTH, W_HEIGHT);

    glEnable(GL_DEPTH_TEST);

    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    // ImGUI setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

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
    camera.set_position(-2.25, 1.25, -2.0);
    camera.set_pitch(-25);
    camera.set_yaw(45);

    // World setup
    std::vector<std::unique_ptr<GameObject>> entities;

    auto light_source = LightSource();
    light_source.set_position(-1.5, 0.75, 0.0);
    light_source.set_scale(0.25, 0.25, 0.25);

    auto cube = std::make_unique<Cube>();
    cube->set_position(0, 0, 0);
    cube->set_material_color({1.0, 0.5, 0.31});
    entities.push_back(std::move(cube));

    
    double delta_t = 0.0;
    double last = glfwGetTime();
    double now = 0.0;
    double this_frame_duration_ms = 0.0;
    double time_to_next_frame_ms = 0.0;

    Utils::CircularBuffer fps_history(100);
    
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        now = glfwGetTime();
        delta_t = now - last;
        last = now;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();

        // Debug Text
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        fps_history.push(1.0f / delta_t);

        ImGui::Text("Mean FPS: %.1f", fps_history.average_value(20));
        ImGui::Text("Min FPS: %.1f", fps_history.min_value(20));
        ImGui::PlotLines("FPS", fps_history.buffer(), fps_history.size(),
                 0, nullptr, 60.0f, 120.0f, ImVec2(0, 80));
        
        ImGui::Separator();
        
        ImGui::Text("Camera;  x: %.2f, y:%.2f, z: %.2f,\n\t pitch:%.2f, yaw:%.2f",
            camera.get_position().x,
            camera.get_position().y,
            camera.get_position().z,
            camera.get_pitch(),
            camera.get_yaw()
        );

        ImGui::Separator();

        ImGui::Text("World Objects:");
        ImGui::BeginChild("Scrolling");

        // Render Loop
        light_source.update(delta_t);
        light_source.render(camera);

        for (const auto& object : entities) {
            ImGui::BulletText("Object: %d; x: %.2f, y:%.2f, z: %.2f\n\t rx:%.2f, ry:%.2f, rz:%.2f",
                object->get_id(),
                object->get_position().x,
                object->get_position().y,
                object->get_position().z,
                object->get_rotation_deg().x,
                object->get_rotation_deg().y,
                object->get_rotation_deg().z
                );
            
            object->set_light_pos(light_source.get_position());
            
            object->update(delta_t);
            object->render(camera);
        }

        ImGui::EndChild();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}

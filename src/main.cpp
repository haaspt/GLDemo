#include <vector>
#include <thread>
#include <chrono>

#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

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

    auto suzanne = GameObject("suzanne.gltf", "default");
    suzanne.rotate_deg(0, 180, 0);

    // Camera Setup
    // OldCamera camera(55.0, static_cast<double>(W_WIDTH) / W_HEIGHT, 0.1, 500.0);
    // camera.set_position(-1.0, 0.75, -3.0);
    // camera.set_pitch(-10);
    // camera.set_yaw(75);

    Camera camera(
                55.0,
                static_cast<double>(W_WIDTH) / W_HEIGHT,
                0.1,
                500.0,
                {-1.0, 0.75, -3.0},
                std::make_unique<FPSController>()
                );


    auto light_source = LightSource("sphere.gltf", Vector3{1.0}, 0.4);
    light_source.set_position(-1.75, 0, -1.75);
    //light_source.set_scale(0.125, 0.125, 0.125);
    //light_source.rotate_deg({180, 0, 0});


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

        fps_history.push(1.0 / delta_t);

        ImGui::Text("Mean FPS: %.1f", fps_history.average_value(20));
        ImGui::Text("Min FPS: %.1f", fps_history.min_value(20));
        ImGui::PlotLines("FPS", fps_history.buffer(), fps_history.size(),
                         0, nullptr, 60.0f, 120.0f, ImVec2(0, 80));

        ImGui::Separator();

        float ambient_strength = light_source.get_strength();
        ImGui::SliderFloat("Ambient Strength", &ambient_strength, 0.0f, 1.0f);

        ImGui::Separator();

        ImGui::Text("World Objects:");
        ImGui::BeginChild("Scrolling");


        Vector3 current_light_pos = light_source.get_position();
        float light_pos_array[3] = {
            static_cast<float>(current_light_pos.x),
            static_cast<float>(current_light_pos.y),
            static_cast<float>(current_light_pos.z)
        };
        ImGui::SliderFloat3("Light Pos", light_pos_array, -5, 5);


        // Render Loop
        camera.update(delta_t);

        light_source.set_strength(ambient_strength);
        light_source.set_position({light_pos_array[0], light_pos_array[1], light_pos_array[2]});
        light_source.update(delta_t);
        light_source.render(camera, {});

        Vector3 current_model_rot = suzanne.get_rotation_deg();
        float model_rot_array[3] = {
            static_cast<float>(current_model_rot.x),
            static_cast<float>(current_model_rot.y),
            static_cast<float>(current_model_rot.z)
        };
        ImGui::SliderFloat3("Model Rot.", model_rot_array, -180, 180);

        suzanne.set_rotation_deg({
            model_rot_array[0],
            model_rot_array[1],
            model_rot_array[2]
        });
        suzanne.update(delta_t);
        suzanne.render(camera, {&light_source});

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

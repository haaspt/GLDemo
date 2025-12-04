#pragma once

#include <filesystem>
#include <memory>
#include <string>

#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>

#include "engine/utilities/Utils.hpp"
#include "engine/scene/Scene.hpp"
#include "engine/utilities/Input.hpp"
#include "engine/resources/ResourceManager.hpp"
#include "engine/scene/Prefab.hpp"

constexpr double TARGET_FPS = 120.0;
constexpr double FRAME_DURATION_MS = 1.0 / TARGET_FPS * 1000.0;

class Application {
private:
    unsigned int window_width_;
    unsigned int window_height_;
    const std::string window_name_;
    GLFWwindow* window_;

    GLFWwindow* init_window();

protected:
    const std::filesystem::path exe_dir_path_;
    std::unique_ptr<Scene::Scene> main_scene_ = nullptr;
    bool initialized = false;

public:
    Application(char** argv, unsigned int window_width = 1200, unsigned int window_height = 900,
                const std::string &window_name = "Demo Game")
        : window_width_(window_width),
          window_height_(window_height),
          window_name_(window_name),
          window_(init_window()),
          exe_dir_path_(Utils::exe_dir_path_from_argv0(argv[0])) {
        // Set up singletons
        Managers::initialize(exe_dir_path_);
        Input::initialize(window_);
    }

    virtual ~Application() noexcept = default;

    virtual void setup() = 0;

    void set_main_scene(std::unique_ptr<Scene::Scene> scene) { main_scene_ = std::move(scene); }

    void add_prefab_to_scene(const Scene::Prefab &prefab);

    double get_aspect_ratio() const { return static_cast<double>(window_width_) / window_height_; }

    void poll_events() const { Input::poll(); }

    void process_scene(double delta_t) {
        assert(main_scene_);
        main_scene_->update(delta_t);
        main_scene_->render();
    }

    bool loop();
};

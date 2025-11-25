//
// Created by Patrick Haas on 11/10/25.
//

#pragma once

#include <GLFW/glfw3.h>

#include "../math/Vector.hpp"


class Input {
public:
    static void initialize(GLFWwindow* glfw_window) {
        Input::window = glfw_window;
        initialized = true;

        capture_cursor();

        glfwSetCursorPosCallback(window, cursor_position_callback);
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        glfwSetKeyCallback(window, keyboard_callback);
    }

    static Vector3 get_input_vec();

    static Vector2 get_cursor_vec() { return cursor_move_vec_; }

    static void poll();

    static void capture_cursor();

    static void release_cursor();

    static bool is_cursor_captured() { return cursor_captured; }

private:
    static bool initialized;
    static GLFWwindow *window;

    static bool cursor_captured;

    static Vector2 last_cursor_pos_;
    static Vector2 cursor_move_vec_;

    static void cursor_position_callback(GLFWwindow * /*window*/, double /*x_pos*/, double /*y_pos*/) {}

    static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

    static void keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

    static void update_cursor_move_vec_();
};

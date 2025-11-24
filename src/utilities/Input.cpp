//
// Created by Patrick Haas on 11/10/25.
//

#include "utilities/Input.hpp"

bool Input::initialized = false;
GLFWwindow* Input::window;

bool Input::cursor_captured = false;

Vector2 Input::last_cursor_pos_ = Vector2{0};
Vector2 Input::cursor_move_vec_ = Vector2{0};

Vector3 Input::get_input_vec() {
    if (!initialized) {
        throw std::runtime_error("Input not initialized! Call Input::initialize() first.");
    }
    Vector3 input_vector = Vector3::ZERO();
    int key_state = glfwGetKey(window, GLFW_KEY_W);
    if (key_state == GLFW_PRESS) {
        input_vector.y += 1;  // UP
    }
    key_state = glfwGetKey(window, GLFW_KEY_S);
    if (key_state == GLFW_PRESS) {
        input_vector.y -= 1;  // DOWN
    }
    key_state = glfwGetKey(window, GLFW_KEY_A);
    if (key_state == GLFW_PRESS) {
        input_vector.x -= 1;  // LEFT
    }
    key_state = glfwGetKey(window, GLFW_KEY_D);
    if (key_state == GLFW_PRESS) {
        input_vector.x += 1;  // RIGHT
    }
    key_state = glfwGetKey(window, GLFW_KEY_UP);
    if (key_state == GLFW_PRESS) {
        input_vector.z += 1;  // Forward
    }
    key_state = glfwGetKey(window, GLFW_KEY_DOWN);
    if (key_state == GLFW_PRESS) {
        input_vector.z -= 1;  // Back
    }
    return input_vector;
}

void Input::poll() {
    update_cursor_move_vec_();
}

void Input::capture_cursor() {
    if (!cursor_captured) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (glfwRawMouseMotionSupported()) {
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        }
        cursor_captured = true;

        // Reset stored cursor pose
        glfwGetCursorPos(window, &last_cursor_pos_.x, &last_cursor_pos_.y);
    }
}

void Input::release_cursor() {
    if (cursor_captured) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        if (glfwRawMouseMotionSupported()) {
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
        }
        cursor_captured = false;

        // Reset stored cursor pose
        glfwGetCursorPos(window, &last_cursor_pos_.x, &last_cursor_pos_.y);
    }
}

void Input::keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        release_cursor();
    }
}

void Input::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        capture_cursor();
    }
}


void Input::update_cursor_move_vec_() {
    if (!cursor_captured) return;
    Vector2 current_cursor_pos;
    glfwGetCursorPos(window, &current_cursor_pos.x, &current_cursor_pos.y);
    cursor_move_vec_ = current_cursor_pos - last_cursor_pos_;
    last_cursor_pos_ = current_cursor_pos;
}

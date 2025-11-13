//
// Created by Patrick Haas on 11/10/25.
//

#include "Input.hpp"

bool Input::initialized = false;
GLFWwindow* Input::window;

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

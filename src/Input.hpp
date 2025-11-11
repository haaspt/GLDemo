//
// Created by Patrick Haas on 11/10/25.
//

#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


class Input {
public:
    explicit Input(GLFWwindow* window) : window(window) {}

    glm::vec3 get_input_vec() const;

private:
    GLFWwindow* window;
};
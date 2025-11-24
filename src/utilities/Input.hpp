//
// Created by Patrick Haas on 11/10/25.
//

#pragma once

#include <GLFW/glfw3.h>

#include "utilities/Vector.hpp"


class Input {
public:
    static void initialize(GLFWwindow* glfw_window) {
        Input::window = glfw_window;
        initialized = true;
    }

    static Vector3 get_input_vec();

private:
    static bool initialized;
    static GLFWwindow* window;
};
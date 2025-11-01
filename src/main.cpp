#include <stdio.h>
#include <vector>
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include "Shader.hpp"
#include "Mesh.hpp"

const unsigned int W_WIDTH = 800;
const unsigned int W_HEIGHT = 600;

void initWindow(GLFWwindow*& window) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "OpenGL Demo", NULL, NULL);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // TODO set keyboard and resize callbacks

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);
}

int main() {
    GLFWwindow* window = nullptr;
    initWindow(window);

    std::vector<float> verts {
        -0.5, -0.5, 0.0, 0.5, 1.0, 1.0,
         0.0,  0.5, 0.0, 1.0, 0.5, 1.0,
         0.5, -0.5, 0.0, 1.0, 1.0, 0.5,
    };
    std::vector<int> idx {
        0, 1, 2
    };

    Shader shader(
        "/Users/patrickhaas/Developer/GLDemo/src/shaders/vertex.glsl",
        "/Users/patrickhaas/Developer/GLDemo/src/shaders/fragment.glsl"
    );

    Mesh mesh(
        verts, idx, &shader
    );
    
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        mesh.render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}

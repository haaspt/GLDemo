cmake_minimum_required(VERSION 3.20)
project(GLDemo)

set(CMAKE_CXX_STANDARD 17)

if(APPLE)
    add_compile_definitions(GL_SILENCE_DEPRECATION)
endif()

include(FetchContent)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.1
)

FetchContent_MakeAvailable(glfw glm)

add_executable(GLDemo src/main.cpp
        src/Mesh.cpp
        src/Mesh.hpp
        src/Shader.cpp
        src/Shader.hpp
        src/GameObject.cpp
        src/GameObject.hpp
        src/Camera.hpp
        src/main.cpp
)

target_link_libraries(GLDemo
        glfw
        glm::glm
        "-framework OpenGL"
)

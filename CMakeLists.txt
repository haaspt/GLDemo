cmake_minimum_required(VERSION 3.20)
project(GLDemo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


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

# Google Test
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)

FetchContent_MakeAvailable(glfw glm googletest)

add_executable(GLDemo src/main.cpp
        src/Mesh.cpp
        src/Mesh.hpp
        src/Shader.cpp
        src/Shader.hpp
        src/GameObject.cpp
        src/GameObject.hpp
        src/Camera.hpp
        src/main.cpp
        src/Input.cpp
        src/Input.hpp
        src/Utils.hpp
        src/Vector.cpp
        src/Vector.hpp
)

target_link_libraries(GLDemo
        glfw
        glm::glm
        "-framework OpenGL"
)

# Enable testing
enable_testing()

# Test executable
add_executable(tests
        tests/test_vector.cpp
        tests/test_transform.cpp
        src/Vector.cpp
        src/Vector.hpp
        src/Utils.hpp
)

target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(tests
        GTest::gtest
        GTest::gtest_main
        GTest::gmock
        glm::glm
)

# Add test to CTest
add_test(NAME VectorTests COMMAND tests)

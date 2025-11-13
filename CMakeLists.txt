cmake_minimum_required(VERSION 3.20)
project(GLDemo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(APPLE)
  add_compile_definitions(GL_SILENCE_DEPRECATION)
endif()

include(FetchContent)

# Speed up third-party builds
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
)

FetchContent_Declare(glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 1.0.1
)

# nlohmann/json exports target nlohmann_json::nlohmann_json
FetchContent_Declare(json
  URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_Declare(googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)

FetchContent_MakeAvailable(glfw glm json googletest)

# OpenGL linking (portable)
find_package(OpenGL REQUIRED)

add_executable(GLDemo
  src/main.cpp
  src/Mesh.cpp
  src/Mesh.hpp
  src/Shader.cpp
  src/Shader.hpp
  src/GameObject.cpp
  src/GameObject.hpp
  src/Camera.hpp
  src/Input.cpp
  src/Input.hpp
  src/Utils.hpp
  src/Vector.cpp
  src/Vector.hpp
  src/ResourceManager.hpp
  src/ResourceManager.cpp
)

target_include_directories(GLDemo PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(GLDemo PRIVATE
  glfw
  glm::glm
  nlohmann_json::nlohmann_json
  OpenGL::GL
)

if(APPLE)
  target_link_libraries(GLDemo PRIVATE "-framework OpenGL")
endif()

# --- Tests ---
enable_testing()

add_executable(tests
  tests/test_vector.cpp
  tests/test_transform.cpp
  src/Vector.cpp
  src/Vector.hpp
  src/Utils.hpp
)

target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(tests PRIVATE
  GTest::gtest GTest::gtest_main GTest::gmock
  glm::glm
)

# Either simple CTest entry:
add_test(NAME VectorTests COMMAND tests)
# Or nicer automatic discovery (requires CMake ≥3.10):
include(GoogleTest)
gtest_discover_tests(tests)

# --- Runtime assets ---
add_custom_command(TARGET GLDemo POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders
          $<TARGET_FILE_DIR:GLDemo>/shaders
  COMMENT "Copying shaders to runtime directory"
)

add_custom_command(TARGET GLDemo POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_CURRENT_SOURCE_DIR}/src/models
          $<TARGET_FILE_DIR:GLDemo>/models
  COMMENT "Copying models to runtime directory"
)

target_compile_options(GLDemo PRIVATE -Wall -Wextra -Wpedantic)
target_compile_options(tests PRIVATE -Wall -Wextra -Wpedantic)

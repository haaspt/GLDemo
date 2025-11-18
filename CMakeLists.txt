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

FetchContent_Declare(imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.91.5
)

FetchContent_Declare(googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)

# Additional imgui setup
FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
  FetchContent_Populate(imgui)

  add_library(imgui STATIC
          ${imgui_SOURCE_DIR}/imgui.cpp
          ${imgui_SOURCE_DIR}/imgui_demo.cpp
          ${imgui_SOURCE_DIR}/imgui_draw.cpp
          ${imgui_SOURCE_DIR}/imgui_tables.cpp
          ${imgui_SOURCE_DIR}/imgui_widgets.cpp
          # Backend files for GLFW + OpenGL3
          ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
          ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
  )

  target_include_directories(imgui PUBLIC
          ${imgui_SOURCE_DIR}
          ${imgui_SOURCE_DIR}/backends
  )

  target_link_libraries(imgui PUBLIC glfw OpenGL::GL)
endif()

FetchContent_MakeAvailable(glfw glm json imgui googletest)

# OpenGL linking (portable)
find_package(OpenGL REQUIRED)

add_executable(GLDemo
  src/main.cpp
  src/resources/Mesh.cpp
  src/resources/Mesh.hpp
  src/resources/Shader.cpp
  src/resources/Shader.hpp
  src/objects/GameObject.cpp
  src/objects/GameObject.hpp
  src/objects/Camera.hpp
  src/utilities/Input.cpp
  src/utilities/Input.hpp
  src/utilities/Utils.hpp
  src/utilities/Vector.cpp
  src/utilities/Vector.hpp
  src/resources/ResourceManager.cpp
  src/resources/ResourceManager.hpp
  src/objects/Node.cpp
  src/objects/Node.hpp
  src/game/Pyramid.cpp
  src/game/Pyramid.hpp
  src/game/Globals.hpp
  src/game/Cube.cpp
  src/game/Cube.hpp
)

target_include_directories(GLDemo PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(GLDemo PRIVATE
  glfw
  glm::glm
  nlohmann_json::nlohmann_json
  imgui
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
  src/utilities/Vector.cpp
  src/utilities/Vector.hpp
  src/utilities/Utils.hpp
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
        ${CMAKE_SOURCE_DIR}/src/game/shaders
          $<TARGET_FILE_DIR:GLDemo>/shaders
  COMMENT "Copying shaders to runtime directory"
)

add_custom_command(TARGET GLDemo POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/game/models
          $<TARGET_FILE_DIR:GLDemo>/models
  COMMENT "Copying models to runtime directory"
)

target_compile_options(GLDemo PRIVATE -Wall -Wextra -Wpedantic)
target_compile_options(tests PRIVATE -Wall -Wextra -Wpedantic)

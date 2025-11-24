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

FetchContent_Declare(assimp
  GIT_REPOSITORY https://github.com/assimp/assimp.git
  GIT_TAG v6.0.2
)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)

# --- stb_image (header-only) ---
FetchContent_Declare(stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG master
)
FetchContent_MakeAvailable(stb)

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

FetchContent_MakeAvailable(glfw glm assimp  imgui googletest)

# OpenGL linking (portable)
find_package(OpenGL REQUIRED)

add_executable(GLDemo
  src/main.cpp
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
        src/game/Globals.hpp
        src/third_party/stb_image_impl.cpp
  src/resources/Model.cpp
  src/resources/Model.hpp
        src/objects/LightSource.cpp
        src/objects/LightSource.hpp
        src/objects/RenderedObject.hpp
)

target_include_directories(GLDemo PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(GLDemo PRIVATE
  ${stb_SOURCE_DIR}
)

target_link_libraries(GLDemo PRIVATE
  glfw
  assimp::assimp
  glm::glm
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
set(SHADER_DIR ${CMAKE_SOURCE_DIR}/src/game/shaders)
set(MODEL_DIR  ${CMAKE_SOURCE_DIR}/src/game/models)
set(TEXTURE_DIR  ${CMAKE_SOURCE_DIR}/src/game/textures)
set(RUNTIME_ASSET_DIR $<TARGET_FILE_DIR:GLDemo>)

file(GLOB_RECURSE SHADER_FILES CONFIGURE_DEPENDS
     "${SHADER_DIR}/*")
file(GLOB_RECURSE MODEL_FILES  CONFIGURE_DEPENDS
     "${MODEL_DIR}/*")
file(GLOB_RECURSE TEXTURE_FILES  CONFIGURE_DEPENDS
     "${TEXTURE_DIR}/*")

add_custom_target(copy_assets ALL
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:GLDemo>/shaders"
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:GLDemo>/models"
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:GLDemo>/textures"
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${SHADER_DIR}"  "$<TARGET_FILE_DIR:GLDemo>/shaders"
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${MODEL_DIR}"   "$<TARGET_FILE_DIR:GLDemo>/models"
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${TEXTURE_DIR}" "$<TARGET_FILE_DIR:GLDemo>/textures"
        COMMENT "Copying shaders/models/textures to runtime directory"
)
add_dependencies(GLDemo copy_assets)

target_compile_options(GLDemo PRIVATE -Wall -Wextra -Wpedantic)
target_compile_options(tests PRIVATE -Wall -Wextra -Wpedantic)

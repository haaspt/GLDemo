cmake_minimum_required(VERSION 3.20)
project(DemoEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (APPLE)
    add_compile_definitions(GL_SILENCE_DEPRECATION)
endif ()

include(FetchContent)

find_package(OpenGL REQUIRED)

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
if (NOT imgui_POPULATED)
    FetchContent_Populate(imgui)

    add_library(imgui STATIC
            ${imgui_SOURCE_DIR}/imgui.cpp
            ${imgui_SOURCE_DIR}/imgui_demo.cpp
            ${imgui_SOURCE_DIR}/imgui_draw.cpp
            ${imgui_SOURCE_DIR}/imgui_tables.cpp
            ${imgui_SOURCE_DIR}/imgui_widgets.cpp
            ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
            ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    )

    target_include_directories(imgui PUBLIC
            ${imgui_SOURCE_DIR}
            ${imgui_SOURCE_DIR}/backends
    )

    target_link_libraries(imgui PUBLIC glfw OpenGL::GL)
endif ()

FetchContent_MakeAvailable(glfw glm assimp imgui googletest)

# ---Engine---

add_library(engine
        src/engine/resources/Shader.cpp
        src/engine/resources/Shader.hpp
        src/engine/objects/GameObject.cpp
        src/engine/objects/GameObject.hpp
        src/engine/utilities/Input.cpp
        src/engine/utilities/Input.hpp
        src/engine/utilities/Utils.hpp
        src/engine/math/Vector.hpp
        src/engine/resources/ResourceManager.cpp
        src/engine/resources/ResourceManager.hpp
        src/engine/objects/Node.cpp
        src/engine/objects/Node.hpp
        src/engine/third_party/stb_image_impl.cpp
        src/engine/resources/Model.cpp
        src/engine/resources/Model.hpp
        src/engine/objects/LightSource.cpp
        src/engine/objects/LightSource.hpp
        src/engine/objects/RenderedObject.hpp
        src/engine/controllers/BaseController.hpp
        src/engine/objects/Camera.cpp
        src/engine/objects/Camera.hpp
        src/engine/controllers/FPSController.cpp
        src/engine/controllers/FPSController.hpp
        src/engine/math/Transform.cpp
        src/engine/math/Transform.hpp
        src/engine/math/Quaternion.cpp
        src/engine/math/Quaternion.hpp
        src/engine/math/Utils.hpp
        src/engine/resources/Skybox.cpp
        src/engine/resources/Skybox.hpp
        src/engine/scene/Scene.hpp
        src/engine/controllers/FollowController.cpp
        src/engine/controllers/FollowController.hpp
        src/engine/scene/Prefab.hpp
        src/engine/application/Application.cpp
        src/engine/application/Application.hpp
)

target_include_directories(engine PUBLIC
        ${CMAKE_SOURCE_DIR}/src
        ${stb_SOURCE_DIR}
)

target_link_libraries(engine PUBLIC
        glfw
        assimp::assimp
        glm::glm
        OpenGL::GL
)

if (APPLE)
    target_link_libraries(engine PUBLIC "-framework OpenGL")
endif ()

target_compile_options(engine PRIVATE -Wall -Wextra -Wpedantic)

# --- Game ---
add_executable(game
        src/game/main_game.cpp
        src/game/Globals.hpp
        src/game/SpaceDemo.hpp
)

target_link_libraries(game PRIVATE engine)

target_compile_options(game PRIVATE -Wall -Wextra -Wpedantic)

# --- Editor ---
add_executable(editor
        src/editor/main_editor.cpp
)

target_link_libraries(editor PRIVATE
        engine
        imgui
)

target_compile_options(editor PRIVATE -Wall -Wextra -Wpedantic)

# --- Tests ---
enable_testing()

file(GLOB TEST_SOURCES CONFIGURE_DEPENDS tests/*.cpp)

add_executable(tests
        ${TEST_SOURCES}
)

target_link_libraries(tests PRIVATE
        engine
        GTest::gtest_main
        GTest::gmock
)

include(GoogleTest)
gtest_discover_tests(tests)

target_compile_options(tests PRIVATE -Wall -Wextra -Wpedantic)

# --- Runtime assets ---
set(SHADER_DIR ${CMAKE_SOURCE_DIR}/src/game/shaders)
set(MODEL_DIR ${CMAKE_SOURCE_DIR}/src/game/models)
set(TEXTURE_DIR ${CMAKE_SOURCE_DIR}/src/game/textures)
set(RUNTIME_ASSET_DIR $<TARGET_FILE_DIR:game>)

file(GLOB_RECURSE SHADER_FILES CONFIGURE_DEPENDS
        "${SHADER_DIR}/*")
file(GLOB_RECURSE MODEL_FILES CONFIGURE_DEPENDS
        "${MODEL_DIR}/*")
file(GLOB_RECURSE TEXTURE_FILES CONFIGURE_DEPENDS
        "${TEXTURE_DIR}/*")

add_custom_target(copy_assets ALL
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:game>/shaders"
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:game>/models"
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:game>/textures"
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${SHADER_DIR}" "$<TARGET_FILE_DIR:game>/shaders"
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${MODEL_DIR}" "$<TARGET_FILE_DIR:game>/models"
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${TEXTURE_DIR}" "$<TARGET_FILE_DIR:game>/textures"
        COMMENT "Copying shaders/models/textures to runtime directory"
)
add_dependencies(game copy_assets)

//
// Created by Patrick Haas on 11/25/25.
//

#pragma once

#include <vector>
#include <memory>

#include <OpenGL/gl3.h>

#include "engine/objects/Camera.hpp"
#include "engine/resources/Shader.hpp"
#include "engine/resources/ResourceManager.hpp"


class Skybox {
private:
    unsigned int texture_id_ = 0;
    std::shared_ptr<Shader> shader_;

    GLuint VAO = 0;
    GLuint VBO = 0;

    std::vector<float> vertex_data_ = {
        // positions
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f
    };

    void init_gl_buffers();

    unsigned int load_cubemap(const std::vector<std::string>& faces);

public:
    Skybox(const std::vector<std::string>& faces) : texture_id_(load_cubemap(faces)) {
        init_gl_buffers();
        shader_ = Managers::shader_manager().get("skybox");
    }

    ~Skybox() noexcept {
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
        if (texture_id_) glDeleteTextures(1, &texture_id_);
    }

    Skybox& operator=(const Skybox&) = delete;

    Skybox(const Skybox&) = delete;

    Skybox(Skybox&& other) noexcept
        : texture_id_(other.texture_id_), shader_(std::move(other.shader_)),
          VAO(other.VAO), VBO(other.VBO) {
        other.VAO = 0;
        other.VBO = 0;
        other.texture_id_ = 0;
        other.shader_ = nullptr;
    }

    Skybox& operator=(Skybox&& other) noexcept {
        if (this != &other) {
            // Delete current
            glDeleteBuffers(1, &VBO);
            glDeleteVertexArrays(1, &VAO);
            if (texture_id_) glDeleteTextures(1, &texture_id_);

            VAO = other.VAO;
            VBO = other.VBO;
            texture_id_ = other.texture_id_;
            shader_ = other.shader_;

            other.VAO = 0;
            other.VBO = 0;
            other.texture_id_ = 0;
            other.shader_ = nullptr;
        }
        return *this;
    }

    void render(const Camera& camera) const;
};

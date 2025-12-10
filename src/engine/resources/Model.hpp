//
// Created by Tyler Haas on 11/21/25.
//

#pragma once

#include <vector>
#include <string>

#include <OpenGL/gl3.h>
#include <assimp/material.h>

#include "engine/resources/Shader.hpp"
#include "engine/resources/Texture.hpp"
#include "engine/math/Vector.hpp"
#include "engine/math/Transform.hpp"


namespace Model {
    class Material {
    private:
        std::shared_ptr<Texture> texture_;
        Vector3 ambient_;
        Vector3 diffuse_;
        Vector3 specular_;
        float shininess_;

    public:
        explicit Material(aiMaterial* ai_material);

        ~Material() noexcept = default;

        Material(const Material&) = delete;

        Material& operator=(const Material&) = delete;

        Material(Material&& other) noexcept = default;

        Material& operator=(Material&& other) noexcept = default;

        bool has_texture() const { return texture_ != nullptr; }
        std::shared_ptr<Texture> get_texture() const { return texture_; }

        Vector3 get_ambient() const { return ambient_; }
        Vector3 get_diffuse() const { return diffuse_; }
        Vector3 get_specular() const { return specular_; }
        float get_shininess() const { return shininess_; }
    };

    class Mesh {
    private:
        GLuint VAO = 0;
        GLuint VBO = 0;
        GLuint EBO = 0;

        // TODO I don't think we need to store this data after gl_init()
        // but I'm keeping it in case I get to a point where we need to
        // dynamically allocate and deallocate the GL buffers.
        std::vector<float> mesh_data = {};  // Interleaved data
        // Pos vec3; Norm vec3; UV vec2;
        std::vector<unsigned int> indices = {};
        unsigned int material_index_ = -1;
        unsigned int index_count_;

        void gl_init();

        friend class Model;

    public:
        Mesh(std::vector<float> mesh_data, std::vector<unsigned int> index_data, unsigned int material_index)
            : mesh_data(std::move(mesh_data)),
              indices(std::move(index_data)),
              material_index_(material_index),
              index_count_(indices.size()) {
            gl_init();
        }

        ~Mesh() noexcept;

        Mesh(const Mesh&) = delete;

        Mesh& operator=(const Mesh&) = delete;

        // Implement move operations
        Mesh(Mesh&& other) noexcept
            : VAO(other.VAO), VBO(other.VBO), EBO(other.EBO),
              mesh_data(std::move(other.mesh_data)),
              indices(std::move(other.indices)),
              material_index_(other.material_index_),
              index_count_(other.index_count_) {
            other.VAO = 0;
            other.VBO = 0;
            other.EBO = 0;
        }

        Mesh& operator=(Mesh&& other) noexcept {
            if (this != &other) {
                // Delete current
                glDeleteBuffers(1, &VBO);
                glDeleteBuffers(1, &EBO);
                glDeleteVertexArrays(1, &VAO);

                // Move from other
                VAO = other.VAO;
                VBO = other.VBO;
                EBO = other.EBO;
                mesh_data = std::move(other.mesh_data);
                indices = std::move(other.indices);
                material_index_ = other.material_index_;
                index_count_ = other.index_count_;

                // Void out other
                other.VAO = 0;
                other.VBO = 0;
                other.EBO = 0;
            }
            return *this;
        }

        unsigned int get_material_index() const { return material_index_; }

        void draw() const;
    };

    class Node {
    private:
        std::vector<Node*> children_ = {};
        std::vector<unsigned int> mesh_indices_ = {};
        Transform transform_{};

    public:
        Node(std::vector<unsigned int> mesh_indices,
             const aiMatrix4x4& ai_transform)
            : mesh_indices_(std::move(mesh_indices)),
              transform_(ai_transform) {
        };

        ~Node() noexcept { for (auto child: children_) delete child; }

        Node(const Node&) = delete;

        Node& operator=(const Node&) = delete;

        Node(Node&& other) = delete;

        Node& operator=(Node&&) = delete;

        void add_child(Node* child) { children_.push_back(child); }

        void render(const Transform& parent_transform,
                    const std::vector<Mesh>& mesh_ref,
                    const std::vector<Material>& mat_ref,
                    const Shader& shader_ref) const;
    };

    class Model {
    private:
        std::vector<Mesh> meshes_;
        std::vector<Material> materials_;
        Node* root_node_ = nullptr;

    public:
        explicit Model(const std::string& model_path);

        ~Model() noexcept { delete root_node_; }

        Model(const Model&) = delete;

        Model& operator=(const Model&) = delete;

        Model(Model&& other) noexcept
            : meshes_(std::move(other.meshes_)),
              materials_(std::move(other.materials_)),
              root_node_(other.root_node_) {
            other.root_node_ = nullptr;
        }

        void render(const Transform& model_transform, const Shader& shader_ref) const;
    };
}

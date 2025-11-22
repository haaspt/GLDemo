//
// Created by Tyler Haas on 11/21/25.
//

#pragma once

#include <vector>
#include <string>

#include <assimp/matrix4x4.h>
#include <OpenGL/gl3.h>

#include "resources/Shader.hpp"
#include "utilities/Vector.hpp"

namespace Model {
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
        // TODO material
        unsigned int index_count_;

        void gl_init();

        friend class Node;
        friend class Model;

    public:
        Mesh(std::vector<float> mesh_data, std::vector<unsigned int> index_data)
            : mesh_data(std::move(mesh_data)),
              indices(std::move(index_data)),
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
                index_count_ = other.index_count_;

                // Void out other
                other.VAO = 0;
                other.VBO = 0;
                other.EBO = 0;
            }
            return *this;
        }

        void draw() const;
    };

    class Node {
    private:
        std::vector<Node*> children_ = {};
        std::vector<unsigned int> mesh_indices_ = {};
        Transform transform_{};

    public:
        explicit Node(unsigned int index) : mesh_indices_{index}, transform_(Transform()) {
        };

        Node(unsigned int index, const aiMatrix4x4& ai_transform) : mesh_indices_{index}, transform_(ai_transform) {
        };

        Node(std::vector<unsigned int> indices, const aiMatrix4x4& ai_transform)
            : mesh_indices_(std::move(indices)), transform_(ai_transform) {
        };

        ~Node() noexcept { for (auto child: children_) delete child; }

        Node(const Node&) = delete;

        Node& operator=(const Node&) = delete;

        Node(Node&& other) = delete;

        Node& operator=(Node&&) = delete;

        void add_child(Node* child) { children_.push_back(child); }

        void render(const Transform& parent_transform, const std::vector<Mesh>& mesh_ref,
                    const Shader& shader_ref) const;
    };

    class Model {
    private:
        std::vector<Mesh> meshes_;
        Node* root_node_ = nullptr;

    public:
        explicit Model(const std::string& model_path);

        ~Model() noexcept { delete root_node_; }

        Model(const Model&) = delete;

        Model& operator=(const Model&) = delete;

        Model(Model&& other) noexcept
            : meshes_(std::move(other.meshes_)),
              root_node_(other.root_node_) {
            other.root_node_ = nullptr;
        }

        void render(const Transform& model_transform, const Shader& shader_ref) const;
    };
}

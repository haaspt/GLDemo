//
// Created by Tyler Haas on 11/21/25.
//

#pragma once

#include <vector>
#include <string>

#include <assimp/matrix4x4.h>
#include <OpenGL/gl3.h>

#include "utilities/Vector.hpp"

namespace Model {

class Mesh {
private:
    GLuint VAO, VBO, EBO;


    std::vector<float> mesh_data = {};  // Interleaved data
    // Pos vec3; Norm vec3; UV vec2;
    std::vector<unsigned int> indices = {};
    // TODO material
    unsigned int index_count_;

    void gl_init();

    friend class Node;
    friend class Model;
public:
    Mesh(const std::vector<float>& mesh_data, const std::vector<unsigned int>& index_data)
        : mesh_data(std::move(mesh_data)), indices(std::move(index_data)), index_count_(index_data.size()) {
        gl_init();
    }
    ~Mesh() noexcept;
    void draw() const;
};

class Node {
private:
    std::vector<Node*> children_ = {};
    std::vector<unsigned int> mesh_indices_ = {};
    Transform transform_{};
public:
    explicit Node(unsigned int index) : mesh_indices_{index}, transform_(Transform()) {};
    Node(unsigned int index, const aiMatrix4x4& ai_transform) : mesh_indices_{index}, transform_(ai_transform) {};
    Node(std::vector<unsigned int> indices, const aiMatrix4x4& ai_transform)
        : mesh_indices_(std::move(indices)), transform_(ai_transform) {};

    ~Node() noexcept {for (auto child : children_) delete child;}

    void add_child(Node* child) {children_.push_back(child);}
    void render(const Transform& parent_transform, const std::vector<Mesh>& mesh_ref) const;
};

class Model {
private:
    std::vector<Mesh> meshes_;
    Node* root_node_ = nullptr;
public:
    explicit Model(const std::string& model_path);
    ~Model() noexcept {delete root_node_;}

    void render(const Transform& model_transform) const;
};
}
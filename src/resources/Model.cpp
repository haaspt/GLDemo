//
// Created by Tyler Haas on 11/21/25.
//

#include "Model.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Model {
Node* create_node_tree(aiNode* ai_node) {
    std::vector<unsigned int> mesh_indices;
    for (unsigned int n_mesh = 0; n_mesh < ai_node->mNumMeshes; n_mesh ++) {
        mesh_indices.push_back(ai_node->mMeshes[n_mesh]);
    }
    Node* new_node = new Node(mesh_indices, ai_node->mTransformation);
    if (ai_node->mNumChildren) {
        for (unsigned int n_child = 0; n_child < ai_node->mNumChildren; n_child++) {
            new_node->add_child(create_node_tree(ai_node->mChildren[n_child]));
        }
    }
    return new_node;
}
Model::Model(const std::string& model_path) {
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(model_path,
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType
    );
    if (scene == nullptr) {
        throw std::runtime_error(
            std::string("Error during model loading: ") + importer.GetErrorString()
            );
    }

    // Load meshes
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        MeshTemp new_mesh;
        for (unsigned int i_v = 0; i_v < scene->mMeshes[i]->mNumVertices; i_v++) {
            new_mesh.vertices.push_back(scene->mMeshes[i]->mVertices[i_v].x);
            new_mesh.vertices.push_back(scene->mMeshes[i]->mVertices[i_v].y);
            new_mesh.vertices.push_back(scene->mMeshes[i]->mVertices[i_v].z);

            new_mesh.normals.push_back(scene->mMeshes[i]->mNormals[i_v].x);
            new_mesh.normals.push_back(scene->mMeshes[i]->mNormals[i_v].y);
            new_mesh.normals.push_back(scene->mMeshes[i]->mNormals[i_v].z);

            new_mesh.uvs.push_back(scene->mMeshes[i]->mTextureCoords[0][i_v].x);
            new_mesh.uvs.push_back(scene->mMeshes[i]->mTextureCoords[0][i_v].y);
        }
        for (unsigned int i_i = 0; i_i < scene->mMeshes[i]->mNumFaces; i_i++) {
            for (unsigned int n_face = 0; n_face < scene->mMeshes[i]->mFaces[i_i].mNumIndices; n_face++) {
                new_mesh.indices.push_back(scene->mMeshes[i]->mFaces[i_i].mIndices[n_face]);
            }
        }
        meshes_.push_back(std::move(new_mesh));
    }

    // Load nodes
    root_node_ = create_node_tree(scene->mRootNode);
}

void Model::render(const Transform& model_transform) const {
    if (root_node_) {
        root_node_->render(model_transform, meshes_);
    }
}

void Node::render(const Transform& parent_transform, const std::vector<MeshTemp>& mesh_ref) const {
    Transform this_trans = parent_transform * transform_;

    // Do render stuff
    for (auto mesh_index : mesh_indices_) {
        const MeshTemp& this_mesh = mesh_ref[mesh_index];
    }

    for (auto child : children_) {
        child->render(this_trans, mesh_ref);
    }
}

}
//
// Created by Tyler Haas on 11/21/25.
//

#include "Model.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "resources/ResourceManager.hpp"

namespace Model {
    Material::Material(aiMaterial* ai_material) {
        if (ai_material->GetTextureCount(aiTextureType_DIFFUSE)) {
            aiString texture_path;
            ai_material->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path);
            std::filesystem::path path = texture_path.C_Str();
            texture_name_ = path.filename();  // for now just assume it's always in the /texture/ folder
            texture_ = Managers::texture_manager().get(texture_name_);
        }

        // TODO Blender uses PBR, not Phong lighting
        // For now this is the best we can do
        aiColor3D ambient, diffuse, specular;
        float shininess;
        // ai_material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
        ai_material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
        // ai_material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
        ai_material->Get(AI_MATKEY_SHININESS, shininess);

        // ambient_.x = ambient.r;
        // ambient_.y = ambient.g;
        // ambient_.z = ambient.b;

        diffuse_.x = diffuse.r;
        diffuse_.y = diffuse.g;
        diffuse_.z = diffuse.b;

        float target = 0.4;
        float blend = 0.4f;
        ambient_.x = diffuse.r + (target - diffuse.r) * blend;
        ambient_.y = diffuse.g + (target - diffuse.g) * blend;
        ambient_.z = diffuse.b + (target - diffuse.b) * blend;

        specular_ = Vector3(1.0);

        // specular_.x = specular.r;
        // specular_.y = specular.g;
        // specular_.z = specular.b;

        shininess_ = shininess;
    }

    Material::~Material() noexcept {
        if (texture_) {
            Managers::texture_manager().release(texture_name_);
        }
    }

    Material::Material(Material&& other) noexcept
        : texture_(other.texture_), texture_name_(std::move(other.texture_name_)) {
        other.texture_ = nullptr;
    }

    Material& Material::operator=(Material&& other) noexcept {
        if (this != &other) {
            Managers::texture_manager().release(texture_name_);

            texture_ = other.texture_;
            texture_name_ = std::move(other.texture_name_);

            other.texture_ = nullptr;
        }
        return *this;
    }


    Node* create_node_tree(aiNode* ai_node) {
        std::vector<unsigned int> mesh_indices;
        mesh_indices.reserve(ai_node->mNumMeshes);
        for (unsigned int n_mesh = 0; n_mesh < ai_node->mNumMeshes; n_mesh++) {
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


        materials_.reserve(scene->mNumMaterials);
        for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
            materials_.emplace_back(scene->mMaterials[i]);
        }

        // Load meshes
        for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
            std::vector<float> mesh_data;
            mesh_data.reserve(scene->mMeshes[i]->mNumVertices * 8);

            for (unsigned int i_v = 0; i_v < scene->mMeshes[i]->mNumVertices; i_v++) {
                mesh_data.push_back(scene->mMeshes[i]->mVertices[i_v].x);
                mesh_data.push_back(scene->mMeshes[i]->mVertices[i_v].y);
                mesh_data.push_back(scene->mMeshes[i]->mVertices[i_v].z);

                mesh_data.push_back(scene->mMeshes[i]->mNormals[i_v].x);
                mesh_data.push_back(scene->mMeshes[i]->mNormals[i_v].y);
                mesh_data.push_back(scene->mMeshes[i]->mNormals[i_v].z);

                if (scene->mMeshes[i]->mTextureCoords[0]) {
                    mesh_data.push_back(scene->mMeshes[i]->mTextureCoords[0][i_v].x);
                    mesh_data.push_back(scene->mMeshes[i]->mTextureCoords[0][i_v].y);
                } else {
                    mesh_data.push_back(0.0f);
                    mesh_data.push_back(0.0f);
                }
            }
            std::vector<unsigned int> index_data;

            for (unsigned int i_i = 0; i_i < scene->mMeshes[i]->mNumFaces; i_i++) {
                for (unsigned int n_face = 0; n_face < scene->mMeshes[i]->mFaces[i_i].mNumIndices; n_face++) {
                    index_data.push_back(scene->mMeshes[i]->mFaces[i_i].mIndices[n_face]);
                }
            }

            meshes_.emplace_back(mesh_data, index_data, scene->mMeshes[i]->mMaterialIndex);
        }

        // Load nodes
        root_node_ = create_node_tree(scene->mRootNode);
    }

    void Model::render(const Transform& model_transform, const Shader& shader_ref) const {
        if (root_node_) {
            root_node_->render(model_transform, meshes_, materials_, shader_ref);
        }
    }

    void Node::render(const Transform& parent_transform, const std::vector<Mesh>& mesh_ref,
                      const std::vector<Material>& mat_ref, const Shader& shader_ref) const {
        Transform this_trans = parent_transform * transform_;

        shader_ref.set_mat4("model", this_trans.to_glm());

        for (auto mesh_index: mesh_indices_) {
            const Mesh& this_mesh = mesh_ref[mesh_index];
            const Material& this_mat = mat_ref[this_mesh.get_material_index()];
            if (this_mat.has_texture()) {
                shader_ref.set_bool("useTexture", true);
                shader_ref.set_int("albedoTex", 0);
                this_mat.get_texture()->bind(0);
            } else {
                shader_ref.set_bool("useTexture", false);
            }
            shader_ref.set_vec3("material_ambient", this_mat.get_ambient().to_glm());
            shader_ref.set_vec3("material_diffuse", this_mat.get_diffuse().to_glm());
            shader_ref.set_vec3("material_specular", this_mat.get_specular().to_glm());
            shader_ref.set_float("material_shininess", this_mat.get_shininess());
            this_mesh.draw();
        }

        for (auto child: children_) {
            child->render(this_trans, mesh_ref, mat_ref, shader_ref);
        }
    }

    Mesh::~Mesh() noexcept {
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
    }

    void Mesh::gl_init() {
        // VAO setup
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // VBO/EBO setup
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(
            GL_ARRAY_BUFFER,
            mesh_data.size() * sizeof(float),
            mesh_data.data(),
            GL_STATIC_DRAW
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            index_count_ * sizeof(unsigned int),
            indices.data(),
            GL_STATIC_DRAW
        );

        // Position attribute
        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            8 * sizeof(float),
            (void*) 0  // Starting at index 0
        );
        glEnableVertexAttribArray(0);

        // Normal attribute
        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            8 * sizeof(float),
            (void*) (3 * sizeof(float))  // offset by 3 preceding pos floats
        );
        glEnableVertexAttribArray(1);

        // UV attribute
        glVertexAttribPointer(
            2,
            2,
            GL_FLOAT,
            GL_FALSE,
            8 * sizeof(float),
            (void*) (6 * sizeof(float))
        );
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);  // unset VAO
    }

    void Mesh::draw() const {
        glBindVertexArray(VAO);
        glDrawElements(
            GL_TRIANGLES,
            index_count_,
            GL_UNSIGNED_INT,
            0
        );
        glBindVertexArray(0);
    }
}

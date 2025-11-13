#pragma once
#include <string>
#include "Node.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "ResourceManager.hpp"

class GameObject : public Node {
private:
    Mesh* mesh = nullptr;
    Shader* shader = nullptr;

    std::string model_name;
    std::string shader_name;
public:
    GameObject(const std::string& model_name, const std::string& shader_name)
        : model_name(model_name), shader_name(shader_name) {
        mesh = Managers::mesh_manager().get(model_name);
        shader = Managers::shader_manager().get(shader_name);
    };
    ~GameObject() noexcept override {
        if (mesh) {
            Managers::mesh_manager().release(model_name);
        }
        if (shader) {
            Managers::shader_manager().release(shader_name);
        }
    }

    // No copy
    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;

    GameObject(GameObject&& other) noexcept
        : Node(std::move(other)),
          mesh(other.mesh),
          shader(other.shader),
          model_name(std::move(other.model_name)),
          shader_name(std::move(other.shader_name)) {
            other.shader = nullptr;
            other.mesh = nullptr;
            other.model_name.clear();
            other.shader_name.clear();
        }
    GameObject& operator=(GameObject&& other) noexcept {
        if (this != &other) {
            Node::operator=(std::move(other));

            if (mesh) {
                Managers::mesh_manager().release(model_name);
            }
            if (shader) {
                Managers::shader_manager().release(shader_name);
            }
            mesh = other.mesh;
            shader = other.shader;
            model_name = std::move(other.model_name);
            shader_name = std::move(other.shader_name);

            other.shader = nullptr;
            other.mesh = nullptr;
            other.model_name.clear();
            other.shader_name.clear();
        }
        return *this;
    }

    void render(const Camera& camera) const;

    void process(double const /*delta_t*/) override {}  // For now, do nothing
};

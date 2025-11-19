#pragma once

#include <string>

#include "objects/Node.hpp"
#include "objects/Camera.hpp"
#include "resources/Mesh.hpp"
#include "resources/Shader.hpp"
#include "resources/ResourceManager.hpp"
#include "utilities/Vector.hpp"

class GameObject : public Node {
private:
    Mesh* mesh = nullptr;
    Shader* shader = nullptr;

    std::string model_name;
    std::string shader_name;

    Vector3 material_color{1.0};
    Vector3 light_color{1.0};
    Vector3 light_pos{0.0};
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

    void set_material_color(const Vector3& color) {material_color = color;}
    Vector3 get_material_color() const {return material_color;}

    void set_light_color(const Vector3& color) {light_color = color;}
    Vector3 get_light_color() const {return light_color;}

    void set_light_pos(const Vector3& pos) {light_pos = pos;}
    Vector3 get_light_pos() const {return light_pos;}

    void render(const Camera& camera) const;

    void process(double const /*delta_t*/) override {}  // For now, do nothing
};

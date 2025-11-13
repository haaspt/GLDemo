#pragma once
#include <string>
#include "Vector.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "ResourceManager.hpp"

class GameObject {
public:
    GameObject(Mesh* mesh, const std::string& shader_name)
        : mesh(mesh), shader_name(shader_name) {
        // TODO mesh handling
        shader = Managers::shader_manager().get(shader_name);
    };
    ~GameObject() noexcept {
        if (shader) {
            // TODO mesh handling
            Managers::shader_manager().release(shader_name);
        }
    }

    // No copy
    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;

    GameObject(GameObject&& other) noexcept
        : mesh(other.mesh)
        , shader(other.shader)
        , shader_name(std::move(other.shader_name))
        , velocity(other.velocity)
        , position(other.position)
        , scale(other.scale)
        , rotation_rad(other.rotation_rad)
        , transform(other.transform) {
        other.shader = nullptr;
        other.mesh = nullptr;
        other.shader_name.clear();
    }
    GameObject& operator=(GameObject&& other) noexcept {
        if (this != &other) {
            // Release current resources
            if (shader) {
                // TODO mesh handling
                Managers::shader_manager().release(shader_name);
            }
            mesh = other.mesh;
            shader = other.shader;
            shader_name = std::move(other.shader_name);
            velocity = other.velocity;
            position = other.position;
            scale = other.scale;
            rotation_rad = other.rotation_rad;
            transform = other.transform;

            other.shader = nullptr;
            other.mesh = nullptr;
            other.shader_name.clear();
        }
        return *this;
    }

    void render(const Camera& camera) const;

    void update(double delta_t);

    Vector3 get_velocity() const {
        return velocity;
    }

    void set_velocity(Vector3 vel) {
        velocity = vel;
    }

    void set_velocity(double const x_vel, double const y_vel, double const z_vel) {
        set_velocity({x_vel, y_vel, z_vel});
    }

    Vector3 get_position() const {
        return position;
    }
    void set_position(Vector3 const pos) {
        position = pos;
    }
    void set_position(double const x, double const y, double const z) {
        set_position({x, y, z});
    }

    Vector3 get_scale() const {
        return scale;
    }
    void set_scale(Vector3 scl) {
        scale = scl;
    }
    void set_scale(double const x, double const y, double const z) {
        set_scale({x, y, z});
    }

    Vector3 get_rotation_rad() const {
        return rotation_rad;
    }
    void set_rotation_rad(Vector3 rot_rad) {
        rotation_rad = rot_rad;
    }
    void set_rotation_rad(double const x_rad, double const y_rad, double const z_rad) {
        set_rotation_rad({x_rad, y_rad, z_rad});
    }
    Vector3 get_rotation_deg() const {
        return rotation_rad.to_degrees();
    }
    void set_rotation_deg(Vector3 rot_deg) {
        set_rotation_rad(rot_deg.to_radians());
    }
    void set_rotation_deg(double const x_deg, double const y_deg, double const z_deg) {
        set_rotation_rad(Vector3(x_deg, y_deg, z_deg).to_radians());
    }

    void rotate_rad(const Vector3& rot_rad) {
        rotation_rad += rot_rad;
    }
    void rotate_rad(double const x_rad, double const y_rad, double const z_rad) {
        rotate_rad({x_rad, y_rad, z_rad});
    }
    void rotate_deg(Vector3 rot_deg) {
        rotate_rad(rot_deg.to_radians());
    }
    void rotate_deg(double const x_deg, double const y_deg, double const z_deg) {
        rotate_rad(Vector3(x_deg, y_deg, z_deg).to_radians());
    }

private:
    Mesh* mesh;
    Shader* shader;

    std::string shader_name;

    Vector3 velocity = Vector3(0.0);
    Vector3 position = Vector3(0.0);
    Vector3 scale = Vector3(1.0);
    Vector3 rotation_rad = Vector3(0.0);

    Transform transform = Transform(1.0);

    void update_transform();
};

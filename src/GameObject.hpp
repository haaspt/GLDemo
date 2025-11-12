#pragma once
#include <string>
#include <glm/glm.hpp>
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

class GameObject {
public:
    GameObject(Mesh* mesh, Shader* shader)
        : mesh(mesh), shader(shader) {};

    void render(const Camera& camera) const;

    void update(float delta_t);

    glm::vec3 get_velocity() const {
        return velocity;
    }

    void set_velocity(glm::vec3 const vel) {
        velocity = vel;
    }

    void set_velocity(float const x_vel, float const y_vel, float const z_vel) {
        set_velocity({x_vel, y_vel, z_vel});
    }

    glm::vec3 get_position() const {
        return position;
    }
    void set_position(glm::vec3 const pos) {
        position = pos;
    }
    void set_position(float const x, float const y, float const z) {
        set_position({x, y, z});
    }

    glm::vec3 get_scale() const {
        return scale;
    }
    void set_scale(glm::vec3 const scl) {
        scale = scl;
    }
    void set_scale(float const x, float const y, float const z) {
        set_scale({x, y, z});
    }

    glm::vec3 get_rotation_rad() const {
        return rotation_rad;
    }
    void set_rotation_rad(glm::vec3 const rot_rad) {
        rotation_rad = rot_rad;
    }
    void set_rotation_rad(float const x_rad, float const y_rad, float const z_rad) {
        set_rotation_rad({x_rad, y_rad, z_rad});
    }
    glm::vec3 get_rotation_deg() const {
        return glm::degrees(rotation_rad);
    }
    void set_rotation_deg(glm::vec3 const rot_deg) {
        set_rotation_rad(glm::radians(rot_deg));
    }
    void set_rotation_deg(float const x_deg, float const y_deg, float const z_deg) {
        set_rotation_rad(glm::radians(glm::vec3(x_deg, y_deg, z_deg)));
    }

    void rotate_rad(glm::vec3 const rot_rad) {
        rotation_rad += rot_rad;
    }
    void rotate_rad(float const x_rad, float const y_rad, float const z_rad) {
        rotate_rad({x_rad, y_rad, z_rad});
    }
    void rotate_deg(glm::vec3 const rot_deg) {
        rotate_rad(glm::radians(rot_deg));
    }
    void rotate_deg(float const x_deg, float const y_deg, float const z_deg) {
        rotate_rad(glm::radians(glm::vec3(x_deg, y_deg, z_deg)));
    }

private:
    Mesh* mesh;
    Shader* shader;

    glm::vec3 velocity = glm::vec3(0.0f);
    glm::vec3 position = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0);
    glm::vec3 rotation_rad = glm::vec3(0.0, 0.0, 0.0);

    glm::mat4 transform = glm::mat4(1.0f);  // Model matrix

    void update_transform();
};

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

    glm::vec3 get_position() const {
        return position;
    }
    void set_position(glm::vec3 pos) {
        position = pos;
        update_transform();
    }
    void set_position(float x, float y, float z) {
        position.x = x;
        position.y = y;
        position.z = z;
        update_transform();
    }

    glm::vec3 get_scale() const {
        return scale;
    }
    void set_scale(glm::vec3 scl) {
        scale = scl;
        update_transform();
    }
    void set_scale(float x, float y, float z) {
        scale.x = x;
        scale.y = y;
        scale.z = z;
        update_transform();
    }

    glm::vec3 get_rotation_rad() const {
        return rotation_rad;
    }
    void set_rotation_rad(glm::vec3 rot_rad) {
        rotation_rad = rot_rad;
        update_transform();
    }
    void set_rotation_rad(float x_rad = 0.0f, float y_rad = 0.0f, float z_rad = 0.0f) {
        rotation_rad.x = x_rad;
        rotation_rad.y = y_rad;
        rotation_rad.z = z_rad;
        update_transform();
    }
    glm::vec3 get_rotation_deg() const {
        return glm::degrees(rotation_rad);
    }
    void set_rotation_deg(glm::vec3 rot_deg) {
        rotation_rad = glm::radians(rot_deg);
        update_transform();
    }
    void set_rotation_deg(float x_deg = 0.0f, float y_deg = 0.0f, float z_deg = 0.0f) {
        rotation_rad = glm::radians(glm::vec3(x_deg, y_deg, z_deg));
        update_transform();
    }

    void rotate_rad(glm::vec3 rot_rad) {
        rotation_rad += rot_rad;
        update_transform();
    }
    void rotate_rad(float x_rad = 0.0f, float y_rad = 0.0f, float z_rad = 0.0f) {
        rotation_rad += glm::vec3(x_rad, y_rad, z_rad);
        update_transform();
    }
    void rotate_deg(glm::vec3 rot_deg) {
        rotation_rad += glm::radians(rot_deg);
        update_transform();
    }
    void rotate_deg(float x_deg = 0.0f, float y_deg = 0.0f, float z_deg = 0.0f) {
        rotation_rad += glm::radians(glm::vec3(x_deg, y_deg, z_deg));
        update_transform();
    }

private:
    Mesh* mesh;
    Shader* shader;
    glm::vec3 position = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0);
    glm::vec3 rotation_rad = glm::vec3(0.0, 0.0, 0.0);

    glm::mat4 transform = glm::mat4(1.0f);  // Model matrix

    void update_transform();
};

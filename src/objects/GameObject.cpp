#include "objects/GameObject.hpp"

void GameObject::render(const Camera& camera) const {
    shader->use();
    shader->set_mat4("model", get_transform().to_glm());
    shader->set_mat4("view", camera.get_view_matrix().to_glm());
    shader->set_mat4("projection", camera.get_projection_matrix().to_glm());
    shader->set_vec3("material_color", material_color.to_glm());
    shader->set_vec3("light_color", light_color.to_glm());
    shader->set_vec3("light_pos", light_pos.to_glm());
    shader->set_vec3("view_pos", camera.get_position().to_glm());
    mesh->draw();
}
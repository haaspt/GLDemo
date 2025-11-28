//
// Created by Patrick Haas on 11/23/25.
//

#include "LightSource.hpp"

void LightSource::render(const Camera* camera, const std::vector<const LightSource*>&) const {
    shader->use();
    shader->set_mat4("model", get_transform().to_glm());
    shader->set_mat4("view", camera->get_view_matrix().to_glm());
    shader->set_mat4("projection", camera->get_projection_matrix().to_glm());
    shader->set_vec3("view_pos", camera->get_position().to_glm());
    shader->set_vec3("light_color", color.to_glm());
    shader->set_vec3("material_color", color.to_glm());
    model->render(get_transform(), *shader);
}

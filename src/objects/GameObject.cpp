#include "objects/GameObject.hpp"

#include "LightSource.hpp"
#include "objects/Camera.hpp"


void GameObject::render(const Camera* camera, const std::vector<const LightSource*>& lights) const {
    shader->use();
    shader->set_mat4("model", get_global_transform().to_glm());
    shader->set_mat4("view", camera->get_view_matrix().to_glm());
    shader->set_mat4("projection", camera->get_projection_matrix().to_glm());
    shader->set_vec3("view_pos", camera->get_position().to_glm());
    for (const auto& light : lights) {
        shader->set_vec3("light_pos", light->get_position().to_glm());
        shader->set_vec3("light_color", light->get_color().to_glm());
        shader->set_float("ambient_strength", light->get_strength());
    }
    model->render(get_global_transform(), *shader);
}

void GameObject::process(double delta_t) {
    if (controller) {
        controller->update(*this, delta_t);
    }
}

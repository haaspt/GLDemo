#include "GameObject.hpp"

void GameObject::render(const Camera& camera) const {
    shader->use();
    shader->set_mat4("model", transform);
    shader->set_mat4("view", camera.get_view_matrix());
    shader->set_mat4("projection", camera.get_projection_matrix());
    mesh->draw();
}

void GameObject::update_transform() {
    transform = glm::mat4(1.0f);
    
    transform = glm::translate(transform, position);
    
    // rotates around the position post translation
    transform = glm::rotate(transform, rotation_rad.y, glm::vec3(0, 1, 0));
    transform = glm::rotate(transform, rotation_rad.x, glm::vec3(1, 0, 0));
    transform = glm::rotate(transform, rotation_rad.z, glm::vec3(0, 0, 1));
    
    transform = glm::scale(transform, scale);
}

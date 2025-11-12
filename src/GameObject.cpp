#include "GameObject.hpp"

void GameObject::render(const Camera& camera) const {
    shader->use();
    shader->set_mat4("model", transform.to_glm());
    shader->set_mat4("view", camera.get_view_matrix().to_glm());
    shader->set_mat4("projection", camera.get_projection_matrix().to_glm());
    mesh->draw();
}

void GameObject::update_transform() {
    transform = Transform(1.0);
    
    transform.translate(position);

    transform.rotate(rotation_rad.y, Vector3(0, 1, 0));
    transform.rotate(rotation_rad.x, Vector3(1, 0, 0));
    transform.rotate(rotation_rad.z, Vector3(0, 0, 1));

    transform.scale(scale);
}

void GameObject::update(double const delta_t) {
    position += velocity * delta_t;
    update_transform();
}

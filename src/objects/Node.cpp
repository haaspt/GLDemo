#include "objects/Node.hpp"

const Transform& Node::get_transform() const {
    if (is_transform_dirty) {
        update_transform();
    }
    return transform;
}

void Node::update_transform() const {
    if (parent_node) {
        transform = parent_node->get_transform();
    } else {
        transform = Transform(1.0);
    }
    
    transform.translate(position);

    transform.rotate(rotation_rad.y, Vector3(0, 1, 0));
    transform.rotate(rotation_rad.x, Vector3(1, 0, 0));
    transform.rotate(rotation_rad.z, Vector3(0, 0, 1));

    transform.scale(scale);
    is_transform_dirty = false;
}

void Node::update(double const delta_t) {
    position += velocity * delta_t;
    set_dirty_flag();
    process(delta_t);
}
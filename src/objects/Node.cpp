#include "objects/Node.hpp"

const Transform& Node::get_transform() const {
    if (is_transform_dirty) {
        update_transform();
    }
    return transform;
}

void Node::update_transform() const {
    transform = Transform(1.0);

    transform.translate(position);

    transform *= Transform(rotation);

    transform.scale(scale);
    is_transform_dirty = false;
}

void Node::update(double const delta_t) {
    process(delta_t);
    position += velocity * delta_t;
    set_dirty_flag();
}

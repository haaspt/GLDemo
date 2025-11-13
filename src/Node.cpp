#include "Node.hpp"

void Node::update_transform() {
    transform = Transform(1.0);
    
    transform.translate(position);

    transform.rotate(rotation_rad.y, Vector3(0, 1, 0));
    transform.rotate(rotation_rad.x, Vector3(1, 0, 0));
    transform.rotate(rotation_rad.z, Vector3(0, 0, 1));

    transform.scale(scale);
}

void Node::update(double const delta_t) {
    position += velocity * delta_t;
    update_transform();
    process(delta_t);
}
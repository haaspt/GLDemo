#include "engine/objects/Node.hpp"
#include "engine/scene/Scene.hpp"

void Node::mark_children_for_deletion() const {
    for (auto child_id: children_) {
        scene->get_scene_object(child_id).set_for_deletion();
    }
}

void Node::set_transform_dirty(bool global) const {
    if (global) {
        is_global_transform_dirty = true;
    } else {
        is_local_transform_dirty = true;
    }
    for (const auto child_id: children_) {
        scene->get_scene_object(child_id).set_transform_dirty(true);
    }
}

void Node::set_for_deletion() {
    should_be_deleted = true;
    for (auto child_id: children_) {
        scene->get_scene_object(child_id).set_for_deletion();
    }
}

Node::NodeId Node::add_child(NodeId child_id) {
    assert(scene);
    assert(child_id != id);
    children_.insert(child_id);
    auto& child = scene->get_scene_object(child_id);
    assert(!child.parent_id);
    child.parent_id = id;
    child.set_transform_dirty(true);
    return child_id;
}

bool Node::detach_child(NodeId child_id) {
    assert(scene);
    auto& child = scene->get_scene_object(child_id);
    child.parent_id = 0;
    return children_.erase(child_id);
}

bool Node::detach_from_parent() const {
    if (!parent_id) {
        return false;
    }
    auto& parent = scene->get_scene_object(parent_id);
    set_transform_dirty(true);
    return parent.children_.erase(id);
}


bool Node::remove_child(NodeId child_id) {
    assert(scene);
    assert(children_.erase(child_id) > 0);
    auto& child = scene->get_scene_object(child_id);
    child.set_for_deletion();
    child.mark_children_for_deletion();
    return true;
}

const Transform& Node::get_local_transform() const {
    if (is_local_transform_dirty) {
        update_local_transform();
    }
    return local_transform;
}

const Transform& Node::get_global_transform() const {
    if (is_global_transform_dirty) {
        update_global_transform();
    }
    return global_transform;
}

void Node::update_local_transform() const {
    local_transform = Transform(1.0);
    local_transform.translate(position);
    local_transform *= Transform(rotation);
    local_transform.scale(scale);

    is_local_transform_dirty = false;
}

void Node::update_global_transform() const {
    if (parent_id) {
        global_transform = scene->get_scene_object(parent_id).get_global_transform();
    } else {
        global_transform = Transform(1.0);
    }

    global_transform = global_transform * get_local_transform();

    is_local_transform_dirty = false;
}

void Node::update(double const delta_t) {
    process(delta_t);
    if (velocity.magnitude() != 0.0) {
        position += velocity * delta_t;
        set_transform_dirty();
    }
}

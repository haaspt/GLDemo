#include "objects/Node.hpp"
#include "scene/Scene.hpp"

void Node::mark_children_for_deletion() const {
    for (auto child_id: children_) {
        scene->get_scene_object(child_id)->set_for_deletion();
    }
}


void Node::set_dirty_flag() const {
    is_transform_dirty = true;
    for (auto child_id: children_) {
        scene->get_scene_object(child_id)->set_dirty_flag();
    }
}

void Node::set_for_deletion() {
    should_be_deleted = true;
    for (auto child_id: children_) {
        scene->get_scene_object(child_id)->set_for_deletion();
    }
}

Node::NodeId Node::add_child(NodeId child_id) {
    assert(scene);
    children_.insert(child_id);
    const auto& child = scene->get_scene_object(child_id);
    assert(!child->parent_id);
    child->parent_id = id;
    return child_id;
}

bool Node::detach_child(NodeId child_id) {
    assert(scene);
    const auto& child = scene->get_scene_object(child_id);
    if (!child) {
        return false;
    }
    child->parent_id = 0;
    return children_.erase(child_id);
}

bool Node::detatch_from_parent() const {
    if (!parent_id) {
        return false;
    }
    auto parent = scene->get_scene_object(parent_id);
    if (!parent) {
        return false;
    }
    return parent->children_.erase(id);
}


bool Node::remove_child(NodeId child_id) {
    assert(scene);
    assert(children_.erase(child_id) > 0);
    auto child = scene->get_scene_object(child_id);
    if (!child) {
        return false;
    }
    child->set_for_deletion();
    child->mark_children_for_deletion();
    return true;
}

const Transform& Node::get_transform() const {
    return transform;
}


const Transform& Node::get_clean_transform() const {
    if (is_transform_dirty) {
        update_transform();
    }
    return transform;
}

void Node::update_transform() const {
    if (parent_id) {
        transform = scene->get_scene_object(parent_id)->get_transform();
    } else {
        transform = Transform(1.0);
    }

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

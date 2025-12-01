#pragma once

#include <unordered_set>

#include "math/Vector.hpp"
#include "math/Transform.hpp"
#include "math/Quaternion.hpp"
#include "utilities/Utils.hpp"

namespace Scene {
    class Scene;
}

class Node {
    using NodeId = unsigned int;

public:
    enum class SceneProperties : unsigned int {
        NONE = 0,
        RENDERABLE = 1 << 0,
        CAMERA = 1 << 1,
        AREA_LIGHT = 1 << 2,
    };

private:
    friend class Scene::Scene;

    NodeId id;

    mutable Transform local_transform = Transform(1.0);
    mutable Transform global_transform = Transform(1.0);

    mutable bool is_local_transform_dirty = false;
    mutable bool is_global_transform_dirty = true;

    bool should_be_deleted = false;

    void mark_children_for_deletion() const;

    void update_local_transform() const;
    void update_global_transform() const;

    Vector3 velocity = Vector3(0.0);
    Vector3 position = Vector3(0.0);
    Vector3 scale = Vector3(1.0);
    Quaternion rotation = Quaternion();

protected:
    NodeId parent_id = 0;
    Scene::Scene* scene = nullptr;
    std::unordered_set<NodeId> children_;

    SceneProperties properties;

    void set_transform_dirty(bool global=false) const;

    void set_for_deletion();

    virtual void process(double const/*delta_t*/) {
    }

public:
    Node() : id(Utils::IdGen::get_id()), properties(SceneProperties::NONE) {
    }

    virtual ~Node() noexcept { children_.clear(); }

    Node(const Node&) = delete;

    Node& operator=(const Node&) = delete;

    Node(Node&&) noexcept = default;

    Node& operator=(Node&&) noexcept = default;

    NodeId add_child(NodeId child_id);

    bool remove_child(NodeId child_id);

    bool detach_child(NodeId child_id);

    const std::unordered_set<NodeId>& get_children() const {
        return children_;
    }

    bool detatch_from_parent() const;

    unsigned int get_id() const { return id; }
    SceneProperties get_properties() const { return properties; }

    virtual void update(double delta_t);

    Vector3 get_velocity() const {
        return velocity;
    }

    void set_velocity(Vector3 vel) {
        velocity = vel;
        if (!is_local_transform_dirty) {
            set_transform_dirty();
        }
    }

    void set_velocity(double const x_vel, double const y_vel, double const z_vel) {
        set_velocity({x_vel, y_vel, z_vel});
    }

    Vector3 get_position() const {
        return get_local_transform().get_translation();
    }

    Vector3 get_global_position() const {
        return get_global_transform().get_translation();
    }

    void set_position(Vector3 const pos) {
        position = pos;
        if (!is_local_transform_dirty) {
            set_transform_dirty();
        }
    }

    void set_position(double const x, double const y, double const z) {
        set_position({x, y, z});
    }

    Vector3 get_scale() const {
        return get_local_transform().get_scale();
    }

    void set_scale(Vector3 scl) {
        scale = scl;
        if (!is_local_transform_dirty) {
            set_transform_dirty();
        }
    }

    void set_scale(double const x, double const y, double const z) {
        set_scale({x, y, z});
    }

    void set_rotation_rad(Vector3 rot_rad) {
        rotation = Quaternion::from_euler(rot_rad).normalized();
        if (!is_local_transform_dirty) {
            set_transform_dirty();
        }
    }

    Quaternion get_rotation() const {
        return get_local_transform().get_rotation();
    }

    void set_rotation_rad(double const x_rad, double const y_rad, double const z_rad) {
        set_rotation_rad({x_rad, y_rad, z_rad});
    }

    void set_rotation_deg(Vector3 rot_deg) {
        set_rotation_rad(rot_deg.to_radians());
    }

    void set_rotation_deg(double const x_deg, double const y_deg, double const z_deg) {
        set_rotation_rad(Vector3(x_deg, y_deg, z_deg).to_radians());
    }

    void rotate_rad(const Vector3& rot_rad) {
        Quaternion delta = Quaternion::from_euler(rot_rad);

        // local space
        // rotation = rotation * delta;
        // world space
        // rotation = delta * rotation;

        rotation = (delta * rotation).normalized();
        if (!is_local_transform_dirty) {
            set_transform_dirty();
        }
    }

    void rotate_rad(double const x_rad, double const y_rad, double const z_rad) {
        rotate_rad({x_rad, y_rad, z_rad});
    }

    void rotate_deg(Vector3 rot_deg) {
        rotate_rad(rot_deg.to_radians());
    }

    void rotate_deg(double const x_deg, double const y_deg, double const z_deg) {
        rotate_rad(Vector3(x_deg, y_deg, z_deg).to_radians());
    }

    const Transform& get_local_transform() const;
    const Transform& get_global_transform() const;
};

constexpr Node::SceneProperties operator|(Node::SceneProperties a, Node::SceneProperties b) {
    return static_cast<Node::SceneProperties>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
}

constexpr Node::SceneProperties operator&(Node::SceneProperties a, Node::SceneProperties b) {
    return static_cast<Node::SceneProperties>(static_cast<unsigned int>(a) & static_cast<unsigned int>(b));
}

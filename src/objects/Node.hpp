#pragma once

#include <unordered_set>

#include "math/Vector.hpp"
#include "math/Transform.hpp"
#include "math/Quaternion.hpp"
#include "utilities/Utils.hpp"
#include "controllers/BaseController.hpp"

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

    std::unique_ptr<BaseController> controller_ = nullptr;

    bool get_transform_dirty_state(bool global = false) const {
        if (global) return is_global_transform_dirty;
        return is_local_transform_dirty;
    }

    void set_transform_dirty(bool global = false) const;

    void set_for_deletion();

    virtual void process(double const/*delta_t*/) {
    }

public:
    Node() : id(Utils::IdGen::get_id()), properties(SceneProperties::NONE) {
    }

    virtual ~Node() noexcept = default;

    Node(const Node& ) = delete;

    Node& operator=(const Node& ) = delete;

    Node(Node&&) noexcept = default;

    Node& operator=(Node&&) noexcept = default;

    NodeId add_child(NodeId child_id);

    bool remove_child(NodeId child_id);

    bool detach_child(NodeId child_id);

    const std::unordered_set<NodeId>& get_children() const {
        return children_;
    }

    bool detach_from_parent() const;

    NodeId get_id() const { return id; }
    SceneProperties get_properties() const { return properties; }

    virtual void update(double delta_t);

    Vector3 get_velocity() const {
        return velocity;
    }

    Node& set_velocity(Vector3 vel) {
        velocity = vel;
        if (!is_local_transform_dirty) {
            set_transform_dirty();
        }
        return *this;
    }

    Node& set_velocity(double const x_vel, double const y_vel, double const z_vel) {
        return set_velocity({x_vel, y_vel, z_vel});
    }

    Vector3 get_position() const {
        return get_local_transform().get_translation();
    }

    Vector3 get_global_position() const {
        return get_global_transform().get_translation();
    }

    Node& set_position(Vector3 const pos) {
        position = pos;
        if (!is_local_transform_dirty) {
            set_transform_dirty();
        }
        return *this;
    }

    Node& set_position(double const x, double const y, double const z) {
        return set_position({x, y, z});
    }

    Vector3 get_scale() const {
        return get_local_transform().get_scale();
    }

    Node& set_scale(Vector3 scl) {
        scale = scl;
        if (!is_local_transform_dirty) {
            set_transform_dirty();
        }
        return *this;
    }

    Node& set_scale(double const x, double const y, double const z) {
        return set_scale({x, y, z});
    }

    Node& set_rotation(const Quaternion &rot) {
        rotation = rot;
        if (!is_local_transform_dirty) {
            set_transform_dirty();
        }
        return *this;
    }

    Node& set_rotation_rad(Vector3 rot_rad) {
        return set_rotation(Quaternion::from_euler(rot_rad).normalized());
    }

    Quaternion get_rotation() const {
        return get_local_transform().get_rotation();
    }

    Node& set_rotation_rad(double const x_rad, double const y_rad, double const z_rad) {
        return set_rotation_rad({x_rad, y_rad, z_rad});
    }

    Node& set_rotation_deg(Vector3 rot_deg) {
        return set_rotation_rad(rot_deg.to_radians());
    }

    Node& set_rotation_deg(double const x_deg, double const y_deg, double const z_deg) {
        return set_rotation_rad(Vector3(x_deg, y_deg, z_deg).to_radians());
    }

    Node& rotate(const Quaternion &delta) {
        rotation = (delta * rotation).normalized();
        if (!is_local_transform_dirty) {
            set_transform_dirty();
        }
        return *this;
    }

    Node& rotate_rad(const Vector3 &rot_rad) {
        Quaternion delta = Quaternion::from_euler(rot_rad);

        return rotate(delta);
    }

    Node& rotate_rad(double const x_rad, double const y_rad, double const z_rad) {
        return rotate_rad({x_rad, y_rad, z_rad});
    }

    Node& rotate_deg(Vector3 rot_deg) {
        return rotate_rad(rot_deg.to_radians());
    }

    Node& rotate_deg(double const x_deg, double const y_deg, double const z_deg) {
        return rotate_rad(Vector3(x_deg, y_deg, z_deg).to_radians());
    }

    const Transform& get_local_transform() const;

    const Transform& get_global_transform() const;

    virtual void set_controller(std::unique_ptr<BaseController> new_controller) { controller_ = std::move(new_controller); }
};

constexpr Node::SceneProperties operator|(Node::SceneProperties a, Node::SceneProperties b) {
    return static_cast<Node::SceneProperties>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
}

constexpr Node::SceneProperties operator&(Node::SceneProperties a, Node::SceneProperties b) {
    return static_cast<Node::SceneProperties>(static_cast<unsigned int>(a) & static_cast<unsigned int>(b));
}

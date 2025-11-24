#pragma once

#include <unordered_map>

#include "utilities/Vector.hpp"
#include "utilities/Utils.hpp"

class Node {
private:
    unsigned int id;

    mutable Transform transform = Transform(1.0);
    mutable bool is_transform_dirty = false;

    void update_transform() const;

protected:
    Vector3 velocity = Vector3(0.0);
    Vector3 position = Vector3(0.0);
    Vector3 scale = Vector3(1.0);
    Vector3 rotation_rad = Vector3(0.0);

    const Transform& get_transform() const;

    void set_dirty_flag() const { is_transform_dirty = true; }

    virtual void process(double const/*delta_t*/) {
    }

public:
    Node() : id(Utils::IdGen::get_id()) {
    }

    virtual ~Node() noexcept = default;

    Node(const Node&) = delete;

    Node& operator=(const Node&) = delete;

    Node(Node&&) noexcept = default;

    Node& operator=(Node&&) noexcept = default;

    unsigned int get_id() const { return id; }

    void update(double delta_t);

    Vector3 get_velocity() const {
        return velocity;
    }

    void set_velocity(Vector3 vel) {
        velocity = vel;
        set_dirty_flag();
    }

    void set_velocity(double const x_vel, double const y_vel, double const z_vel) {
        set_velocity({x_vel, y_vel, z_vel});
    }

    Vector3 get_position() const {
        return position;
    }

    void set_position(Vector3 const pos) {
        position = pos;
        set_dirty_flag();
    }

    void set_position(double const x, double const y, double const z) {
        set_position({x, y, z});
    }

    Vector3 get_scale() const {
        return scale;
    }

    void set_scale(Vector3 scl) {
        scale = scl;
        set_dirty_flag();
    }

    void set_scale(double const x, double const y, double const z) {
        set_scale({x, y, z});
    }

    Vector3 get_rotation_rad() const {
        return rotation_rad;
    }

    void set_rotation_rad(Vector3 rot_rad) {
        rotation_rad = {
            Utils::wrap_radians(rot_rad.x),
            Utils::wrap_radians(rot_rad.y),
            Utils::wrap_radians(rot_rad.z)
        };
        set_dirty_flag();
    }

    void set_rotation_rad(double const x_rad, double const y_rad, double const z_rad) {
        set_rotation_rad({x_rad, y_rad, z_rad});
    }

    Vector3 get_rotation_deg() const {
        return rotation_rad.to_degrees();
    }

    void set_rotation_deg(Vector3 rot_deg) {
        set_rotation_rad(rot_deg.to_radians());
    }

    void set_rotation_deg(double const x_deg, double const y_deg, double const z_deg) {
        set_rotation_rad(Vector3(x_deg, y_deg, z_deg).to_radians());
    }

    void rotate_rad(const Vector3& rot_rad) {
        rotation_rad.x = Utils::wrap_radians(rotation_rad.x + rot_rad.x);
        rotation_rad.y = Utils::wrap_radians(rotation_rad.y + rot_rad.y);
        rotation_rad.z = Utils::wrap_radians(rotation_rad.z + rot_rad.z);
        set_dirty_flag();
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
};

#pragma once

#include "utilities/Vector.hpp"
#include "utilities/Utils.hpp"

class Camera {
public:
    Camera(double fov_deg, double aspect_ratio, double near_plane_dist, double far_plane_dist)
        : projection(Transform::perspective(
            Utils::to_radians(fov_deg),
            aspect_ratio,
            near_plane_dist,
            far_plane_dist
        )) {
    }

    Vector3 get_position() const {
        return position;
    }

    void set_position(Vector3 pos) {
        position = pos;
        view_dirty = true;
    }

    void set_position(double x, double y, double z) {
        set_position({x, y, z});
    }

    double get_pitch() const {
        return Utils::to_degrees(pitch);
    }

    void set_pitch(double pitch_deg) {
        pitch = Utils::to_radians(pitch_deg);
        view_dirty = true;
    }

    double get_yaw() const {
        return Utils::to_degrees(yaw);
    }

    void set_yaw(double yaw_deg) {
        yaw = Utils::to_radians(yaw_deg);
        view_dirty = true;
    }

    const Transform& get_view_matrix() const {
        if (view_dirty) {
            update_view_matrix();
        }
        return view;
    }

    const Transform& get_projection_matrix() const {
        return projection;
    }

private:
    mutable bool view_dirty = true;
    // TODO: allow FOV and aspect changes

    Vector3 position = Vector3(0.0);
    double yaw = 0.0;
    double pitch = 0.0;

    mutable Transform view = Transform(1.0);
    Transform projection;

    void update_view_matrix() const {
        Vector3 front;
        front.x = cos(yaw) * cos(pitch);
        front.y = sin(pitch);
        front.z = sin(yaw) * cos(pitch);
        front.normalize();

        view = look_at(
            position,
            position + front
        );

        view_dirty = false;
    }
};

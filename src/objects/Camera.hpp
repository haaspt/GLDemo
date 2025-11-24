//
// Created by Patrick Haas on 11/23/25.
//

#pragma once

#include "objects/Node.hpp"
#include "utilities/Vector.hpp"
#include "utilities/Utils.hpp"
#include <controllers/BaseController.hpp>

class Camera : public Node {
private:
    Transform projection;
    std::unique_ptr<BaseController> controller = nullptr;

public:
    Camera(double fov_deg, double aspect_ratio, double near_plane_dist, double far_plane_dist,
           Vector3 pos = Vector3{0.0}, std::unique_ptr<BaseController> controller = {})
        : projection(Transform::perspective(
              Utils::to_radians(fov_deg),
              aspect_ratio,
              near_plane_dist,
              far_plane_dist
          )),
          controller(std::move(controller)) {
        position = pos;
    }

    Camera(Camera&& other) noexcept
        : Node(std::move(other)),
          projection(other.projection),
          controller(std::move(other.controller)) {
        other.controller = nullptr;
    }

    Camera& operator=(Camera&& other) noexcept {
        if (this != &other) {
            Node::operator=(std::move(other));

            projection = other.projection;
            controller = std::move(other.controller);
        }
        return *this;
    }

    void set_controller(std::unique_ptr<BaseController> new_controller) { controller = std::move(new_controller); }

    void update(double delta_t) override;

    const Transform& get_view_matrix() const { return get_transform(); }
    const Transform& get_projection_matrix() const { return projection; }
};

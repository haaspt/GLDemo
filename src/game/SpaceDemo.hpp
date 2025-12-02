//
// Created by Patrick Haas on 12/1/25.
//

#pragma once

#include <memory>

#include "scene/Scene.hpp"
#include "scene/Prefab.hpp"
#include "objects/Camera.hpp"
#include "objects/LightSource.hpp"
#include "objects/GameObject.hpp"
#include "objects/Node.hpp"

class ShipNode : public Node {
private:
    Node* ship_mesh = nullptr;
    Vector2 mesh_tilt;

public:
    void set_ship_mesh(Node* ptr) { ship_mesh = ptr; }

    void update(double delta_t) override {
        if (controller_) controller_->update(*this, delta_t);
        set_position(get_position() + get_velocity());

        if (ship_mesh) {
            Vector2 cursor = Input::get_cursor_vec();
            Vector2 target(-cursor.y, cursor.x);

            double speed = 1.0;
            mesh_tilt += (target - mesh_tilt) * delta_t * speed;

            double pitch_deg = Utils::clamp(mesh_tilt.x * 20.0, -15.0, 10.0);
            double roll_deg = Utils::clamp(mesh_tilt.y * 20.0, -45.0, 45.0);

            ship_mesh->set_rotation_deg(pitch_deg, 0.0, -roll_deg);
        }
    }
};


class ShipController : public BaseController {
private:
    double max_velocity = 1.0;
    double linear_accel = 10.0;
    double linear_decel = 10.0;
    double turn_speed = 0.005;

    double yaw = 0.0;
    double pitch = 0.0;

public:
    void update(Node& node, double dt) override {
        Vector2 cursor = Input::get_cursor_vec();
        yaw += -cursor.x * turn_speed;
        pitch += -cursor.y * turn_speed;

        constexpr double max_pitch = Utils::to_radians(89.0);
        pitch = Utils::clamp(pitch, -max_pitch, max_pitch);

        node.set_rotation_rad({pitch, yaw, 0.0});

        const Transform& t = node.get_local_transform();
        Vector3 forward = t.get_forward();

        Vector3 vel = node.get_velocity();
        Vector3 accel(0, 0, 0);

        Vector3 input = Input::get_input_vec();
        double thrust = std::max(0.0, input.y);   // forward only

        if (thrust > 0.0)
            accel += forward * (thrust * linear_accel);

        if (accel.magnitude() > 0.0) {
            vel += accel * dt;
        } else {
            double speed = vel.magnitude();
            if (speed > 0.0) {
                double decel_amount = linear_decel * dt;
                double new_speed = std::max(0.0, speed - decel_amount);
                vel = vel.normalized() * new_speed;
            }
        }

        if (vel.magnitude() > max_velocity)
            vel = vel.normalized() * max_velocity;

        node.set_velocity(vel);
    }
};

struct ShipPrefab : Scene::Prefab {
    double aspect_ratio;

    Scene::NodeId initialize(Scene::Scene& scene, Scene::NodeId parent_id) override {
        auto root = std::make_unique<ShipNode>();
        root->set_controller(std::make_unique<ShipController>());
        auto root_id = scene.add_scene_object(std::move(root), parent_id);

        auto camera = std::make_unique<Camera>(65.0, aspect_ratio, 0.1, 1000.0, Vector3(0, 5, 15));
        scene.add_scene_object(std::move(camera), root_id);

        auto ship_mesh = std::make_unique<GameObject>("fighter.gltf", "default");
        auto ship_id = scene.add_scene_object(std::move(ship_mesh), root_id);

        dynamic_cast<ShipNode*>(&scene.get_scene_object(root_id))->set_ship_mesh(&scene.get_scene_object(ship_id));

        auto exhaust_light = std::make_unique<LightSource>("sphere.gltf", Vector3{1.0}, 0.4);
        exhaust_light->set_position(0, 0, 3).set_scale(0.5, 0.5, 0.5);
        scene.add_scene_object(std::move(exhaust_light), ship_id);

        return root_id;
    }
};

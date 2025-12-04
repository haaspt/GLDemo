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
    Node *ship_mesh = nullptr;
    Vector2 mesh_tilt;
    Vector2 smoothed_cursor{0};

public:
    void set_ship_mesh(Node *ptr) { ship_mesh = ptr; }

    void update(double delta_t) override {
        if (controller_) controller_->update(*this, delta_t);
        set_position(get_position() + get_velocity());

        if (ship_mesh) {
            Vector2 raw_cursor = Input::get_cursor_vec();

            // Low pass filter, prevents jittery model movement
            double input_smooth_speed = 10.0;
            double alpha = 1.0 - std::exp(-input_smooth_speed * delta_t);

            smoothed_cursor += (raw_cursor - smoothed_cursor) * alpha;

            constexpr double max_visual_pitch_deg = 15.0;
            constexpr double max_visual_roll_deg = 55.0;
            constexpr double max_visual_yaw_deg = 30.0;

            double max_visual_pitch = Utils::to_radians(max_visual_pitch_deg);
            double max_visual_roll = Utils::to_radians(max_visual_roll_deg);
            double max_visual_yaw = Utils::to_radians(max_visual_yaw_deg);

            double target_pitch = Utils::clamp(-smoothed_cursor.y * max_visual_pitch,
                                               -max_visual_pitch,
                                               max_visual_pitch);

            double target_yaw = Utils::clamp(-smoothed_cursor.x * max_visual_yaw,
                                             -max_visual_yaw,
                                             max_visual_yaw);
            double target_roll = Utils::clamp(-smoothed_cursor.x * max_visual_roll,
                                              -max_visual_roll,
                                              max_visual_roll);

            Quaternion target_rot = Quaternion::from_euler(
                Vector3(target_pitch, 0, target_roll)
            );

            Quaternion current = ship_mesh->get_local_transform().get_rotation().normalized();

            double follow_speed = 2.0;
            double w = 1.0 - std::exp(-follow_speed * delta_t);

            Quaternion new_rot = current.slerp(target_rot, w).normalized();
            ship_mesh->set_rotation(new_rot);
        }
    }
};


class ShipController : public BaseController {
private:
    double base_speed = 5.0;
    double max_speed = 25.0;
    double accel_rate = 20.0;
    double strafe_damping = 16.0;
    double turn_speed = 0.25;

    double yaw = 0.0;
    double pitch = 0.0;

public:
    void update(Node &node, double delta_t) override {
        Vector2 cursor_move = Input::get_cursor_vec();

        yaw += -cursor_move.x * turn_speed * delta_t;
        pitch += -cursor_move.y * turn_speed * delta_t;

        // constexpr double max_pitch = Utils::to_radians(89.0);
        // pitch = Utils::clamp(pitch, -max_pitch, max_pitch);

        node.set_rotation_rad({pitch, yaw, 0.0}); // yaw and pitch around, no roll

        const Transform &t = node.get_local_transform();
        Vector3 forward = t.get_forward();

        Vector3 vel = node.get_velocity();

        // Decompose velocity into forward and lateral components
        double forward_speed = vel.dot(forward);
        Vector3 v_forward = forward * forward_speed;
        Vector3 v_lateral = vel - v_forward;


        double thrust_amt = Input::get_input_vec().y;
        double thrust = Utils::clamp(thrust_amt, 0.0, 1.0);

        double target_speed = base_speed + thrust * (max_speed - base_speed);

        // Accel towards target speed
        double speed_delta = target_speed - forward_speed;
        double max_delta = accel_rate * delta_t;

        if (speed_delta > max_delta) speed_delta = max_delta;
        else if (speed_delta < -max_delta) speed_delta = -max_delta;

        forward_speed += speed_delta;
        v_forward = forward * forward_speed;

        // Damp lateral movement
        double damping = std::exp(-strafe_damping * delta_t);
        v_lateral *= damping;

        vel = v_forward + v_lateral;
        node.set_velocity(vel);
    }
};

struct ShipPrefab : Scene::Prefab {
    double aspect_ratio;

    Scene::NodeId initialize(Scene::Scene &scene, Scene::NodeId parent_id) override {
        auto root = std::make_unique<ShipNode>();
        root->set_controller(std::make_unique<ShipController>());
        auto root_id = scene.add_scene_object(std::move(root), parent_id);
        scene.get_scene_object(root_id).set_position(0, 0, 1000);

        auto camera = std::make_unique<Camera>(65.0, aspect_ratio, 0.1, 10000.0, Vector3(0, 5, 15));
        scene.add_scene_object(std::move(camera), root_id);

        auto ship_mesh = std::make_unique<GameObject>("fighter.gltf", "default");
        auto ship_id = scene.add_scene_object(std::move(ship_mesh), root_id);

        dynamic_cast<ShipNode *>(&scene.get_scene_object(root_id))->set_ship_mesh(&scene.get_scene_object(ship_id));

        auto exhaust_light = std::make_unique<LightSource>("sphere.gltf", Vector3{1.0}, 0.4);
        exhaust_light->set_position(0, 0, 3).set_scale(0.5, 0.5, 0.5);
        scene.add_scene_object(std::move(exhaust_light), ship_id);

        return root_id;
    }
};

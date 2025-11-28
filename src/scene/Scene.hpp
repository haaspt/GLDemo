//
// Created by Patrick Haas on 11/27/25.
//

#pragma once

#include <memory>
#include <unordered_set>
#include <vector>

#include "objects/Node.hpp"
#include "objects/Camera.hpp"
#include "objects/LightSource.hpp"
#include "objects/RenderedObject.hpp"
#include "resources/Skybox.hpp"

namespace Scene {
    using NodeId = unsigned int;
    class Scene {
    private:
        std::unordered_map<NodeId, std::unique_ptr<Node>> scene_objects_;
        std::unordered_set<NodeId> area_lights_;
        NodeId scene_camera_;
        std::unique_ptr<Skybox> skybox_ = nullptr;

    public:
        Scene() = default;

        static bool node_has_property(const Node& node, Node::SceneProperties property) {
            return (node.get_properties() & property) != Node::SceneProperties::NONE;
        }

        template<typename T, typename... Args>
        T* create_object(Args&&... args) {
            auto obj = std::make_unique<T>(std::forward<Args>(args)...);
            T* ptr = obj.get();
            add_scene_object(std::move(obj));
            return ptr;
        }

        void create_skybox(const std::vector<std::string>& skybox_textures) {
            skybox_ = std::make_unique<Skybox>(skybox_textures);
        }

        NodeId add_scene_object(std::unique_ptr<Node> node) {
            NodeId id = node->get_id();
            if (node_has_property(*node, Node::SceneProperties::AREA_LIGHT)) {
                area_lights_.insert(id);
            }
            if (node_has_property(*node, Node::SceneProperties::CAMERA)) {
                scene_camera_ = id;
            }
            scene_objects_[id] = std::move(node);
            return id;
        }

        Node* get_scene_object(NodeId id) const {
            auto it = scene_objects_.find(id);
            if (it == scene_objects_.end()) {
                return nullptr;
            }
            return it->second.get();
        }

        void update(double delta_t) const {
            for (const auto& [key, object] : scene_objects_) {
                // TODO check objects for `should_be_removed` flag
                object->update(delta_t);
            }
        }

        void render() const {
            auto it = scene_objects_.find(scene_camera_);
            assert(it != scene_objects_.end());
            auto camera = dynamic_cast<Camera*>(it->second.get());

            if (skybox_) {
                skybox_->render(*camera);
            }

            std::vector<const LightSource*> lights;
            for (auto light_id : area_lights_) {
                auto it = scene_objects_.find(light_id);
                if (it != scene_objects_.end()) {
                    lights.push_back(dynamic_cast<LightSource*>(it->second.get()));
                }
            }

            for (const auto& [key, object] : scene_objects_) {
                if (node_has_property(*object, Node::SceneProperties::RENDERABLE)) {
                    auto* rendered = dynamic_cast<RenderedObject*>(object.get());
                    rendered->render(camera, lights);
                }
            }
        }
    };
}

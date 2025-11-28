//
// Created by Patrick Haas on 11/23/25.
//

#pragma once

#include "objects/Node.hpp"
#include "resources/ResourceManager.hpp"
#include "resources/Model.hpp"
#include "controllers/BaseController.hpp"

class LightSource;
class Camera;


class RenderedObject : public Node {
protected:
    Model::Model* model = nullptr;
    Shader* shader = nullptr;

    std::unique_ptr<BaseController> controller = nullptr;

    std::string model_name;
    std::string shader_name;
    std::string texture_name;

public:
    RenderedObject(const std::string& model_name, const std::string& shader_name,
                   std::unique_ptr<BaseController> controller = {})
        : controller(std::move(controller)), model_name(model_name), shader_name(shader_name) {
        properties = properties | SceneProperties::RENDERABLE;
        model = Managers::model_manager().get(model_name);
        shader = Managers::shader_manager().get(shader_name);
    };

    ~RenderedObject() noexcept override {
        if (model) {
            Managers::model_manager().release(model_name);
        }
        if (shader) {
            Managers::shader_manager().release(shader_name);
        }
    }

    // No copy
    RenderedObject(const RenderedObject&) = delete;

    RenderedObject& operator=(const RenderedObject&) = delete;

    RenderedObject(RenderedObject&& other) noexcept
        : Node(std::move(other)),
          model(other.model),
          shader(other.shader),
          controller(std::move(other.controller)),
          model_name(std::move(other.model_name)),
          shader_name(std::move(other.shader_name)) {
        other.shader = nullptr;
        other.model = nullptr;
        other.controller = nullptr;
        other.model_name.clear();
        other.shader_name.clear();
    }

    RenderedObject& operator=(RenderedObject&& other) noexcept {
        if (this != &other) {
            Node::operator=(std::move(other));

            if (model) {
                Managers::model_manager().release(model_name);
            }
            if (shader) {
                Managers::shader_manager().release(shader_name);
            }
            model = other.model;
            shader = other.shader;
            controller = std::move(other.controller);
            model_name = std::move(other.model_name);
            shader_name = std::move(other.shader_name);

            other.shader = nullptr;
            other.model = nullptr;
            other.controller = nullptr;
            other.model_name.clear();
            other.shader_name.clear();
        }
        return *this;
    }

    void set_controller(std::unique_ptr<BaseController> new_controller) { controller = std::move(new_controller); }

    virtual void render(const Camera* camera, const std::vector<const LightSource*>& lights) const = 0;
};

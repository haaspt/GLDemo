//
// Created by Patrick Haas on 11/23/25.
//

#pragma once

#include "engine/objects/Node.hpp"
#include "engine/resources/ResourceManager.hpp"
#include "engine/resources/Model.hpp"
#include "engine/controllers/BaseController.hpp"

class LightSource;
class Camera;


class RenderedObject : public Node {
protected:
    std::shared_ptr<Model::Model> model;
    std::shared_ptr<Shader> shader;

    std::string model_name;
    std::string shader_name;
    std::string texture_name;

public:
    RenderedObject(const std::string& model_name, const std::string& shader_name,
                   std::unique_ptr<BaseController> controller = {})
        : model_name(model_name), shader_name(shader_name) {
        controller_ = std::move(controller);
        properties = properties | SceneProperties::RENDERABLE;
        model = Managers::model_manager().get(model_name);
        shader = Managers::shader_manager().get(shader_name);
    };

    // No copy
    RenderedObject(const RenderedObject&) = delete;

    RenderedObject& operator=(const RenderedObject&) = delete;

    RenderedObject(RenderedObject&& other) noexcept
        : Node(std::move(other)),
          model(std::move(other.model)),
          shader(std::move(other.shader)),
          model_name(std::move(other.model_name)),
          shader_name(std::move(other.shader_name)),
          texture_name(std::move(other.texture_name)) {
        other.shader = nullptr;
        other.model = nullptr;
        other.model_name.clear();
        other.shader_name.clear();
        other.texture_name.clear();
    }

    RenderedObject& operator=(RenderedObject&& other) noexcept {
        if (this != &other) {
            Node::operator=(std::move(other));
            model = std::move(other.model);
            shader = std::move(other.shader);
            model_name = std::move(other.model_name);
            shader_name = std::move(other.shader_name);
            texture_name = std::move(other.texture_name);

            other.shader = nullptr;
            other.model = nullptr;
            other.model_name.clear();
            other.shader_name.clear();
            other.texture_name.clear();
        }
        return *this;
    }

    virtual void render(const Camera* camera, const std::vector<const LightSource*>& lights) const = 0;
};

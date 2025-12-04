#pragma once

#include "engine/objects/RenderedObject.hpp"

class GameObject : public RenderedObject {
public:
    GameObject(const std::string& model_name, const std::string& shader_name,
               std::unique_ptr<BaseController> controller = {})
        : RenderedObject(model_name, shader_name, std::move(controller)) {
    }

    void render(const Camera* camera, const std::vector<const LightSource*>& lights) const override;

    void process(double delta_t) override;
};

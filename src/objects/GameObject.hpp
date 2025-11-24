#pragma once

#include "RenderedObject.hpp"

class GameObject : public RenderedObject {
public:
    GameObject(const std::string& model_name, const std::string& shader_name)
        : RenderedObject(model_name, shader_name) {}

    void render(const Camera& camera, const std::vector<LightSource*>& lights) const override;

    void process(double const /*delta_t*/) override {};
};

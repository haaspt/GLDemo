//
// Created by Patrick Haas on 11/23/25.
//

#pragma once
#include "engine/objects/GameObject.hpp"
#include "engine/objects/Camera.hpp"


class LightSource : public RenderedObject {
private:
    Vector3 color{1.0};
    float ambient_strength = 1;

public:
    LightSource(const std::string& model_name, const Vector3& color, float ambient_strength,
                std::unique_ptr<BaseController> controller = {})
        : RenderedObject(model_name, "light_source", std::move(controller)),
          color(color),
          ambient_strength(ambient_strength) {
        properties = properties | SceneProperties::AREA_LIGHT | SceneProperties::RENDERABLE;
    };

    Vector3 get_color() const { return color; }
    void set_color(const Vector3& new_color) { color = new_color; }

    float get_strength() const { return ambient_strength; }
    void set_strength(float new_strength) { ambient_strength = new_strength; }

    void render(const Camera* camera, const std::vector<const LightSource*>&) const override;
};

#pragma once

#include "engine/application/Application.hpp"
#include "engine/scene/Prefab.hpp"

struct EditorMainPrefab : public Scene::Prefab {
    double aspect_ratio;

    Scene::NodeId initialize(Scene::Scene &scene) const override;
};

class Editor : public Application {
public:
    Editor(char** argv) : Application(argv, 1200, 900, "Editor") {}

    void setup() override;
};
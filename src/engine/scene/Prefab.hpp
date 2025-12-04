//
// Created by Patrick Haas on 12/1/25.
//

#pragma once
#include "engine/scene/Scene.hpp"

namespace Scene {
    struct Prefab {
        NodeId parent_id = 0;

        virtual ~Prefab() noexcept = default;

        virtual NodeId initialize(Scene &scene) const = 0;
    };
}

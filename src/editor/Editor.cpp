#include "Editor.hpp"

#include "engine/objects/GameObject.hpp"
#include "engine/objects/Camera.hpp"

Scene::NodeId EditorMainPrefab::initialize(Scene::Scene &scene) const {
    auto root_id = scene.create_object<Node>();

    auto cube_id = scene.add_scene_object(std::make_unique<GameObject>("suzanne.gltf", "default"), root_id);

    auto light_id = scene.add_scene_object(std::make_unique<LightSource>("sphere.gltf", Vector3(1), 0.4), root_id);
    scene.get_scene_object(light_id).set_position(5, 5, 5).set_scale(.25, .25, .25);

    auto camera_anchor_id = scene.create_object<Node>();
    auto camera_id = scene.add_scene_object(std::make_unique<Camera>(
    65.0, aspect_ratio, 0.1, 100.0), camera_anchor_id);
    scene.get_scene_object(camera_id).set_position(5, 5, 5).look_at({0,0,0});

    return root_id;

}


void Editor::setup() {
    main_scene_ = std::make_unique<Scene::Scene>();

    auto main_prefab = EditorMainPrefab();
    main_prefab.aspect_ratio = get_aspect_ratio();
    main_prefab.initialize(*main_scene_);

    initialized = true;
}

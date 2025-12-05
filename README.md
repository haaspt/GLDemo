# Overview

This project is my attempt to learn the principles of 3D game development by building an engine from the ground up. It also represents my first proper foray into C++, so it's almost certainly littered with bad practices and wonky code.

## Building

This project uses CMake. It can be built and compiled using the following commands:
```bash
cmake -B build
cmake --build build
```

This will create three executables:
1. `./build/game`: the demo game I've created with the engine
2. `./build/editor`: the (currently feature-lacking) editor
3. `./build/tests`: some unittests, mostly for vector/matrix math and transformation logic


## Project Structure

Under `src/` you will find three directories: `engine/`, `game/`, and `editor/`.

### Engine
`engine/` contains the core engine code, which is shipped as a library. It currently lacks a namespace (as well as a name) but that may change in the future.

The engine is structured around a series of high-level classes that can be derived and used by a developer to create game objects and logic. It may be easiest to look at the demo in `game/`, but here's a brief overview of the concepts.

#### Application
This is the primary container for the game. It manages and owns the GLTF `window`, inits the engine singletons, and runs the main loop. In order to create a game the user creates a class that derives from `Application` and implements the `setup()` virtual method.

`setup()` should do the following:
1. Create a main scene using `set_main_scene()`, which takes a `unique_ptr` to a `Scene::Scene` object.
2. Add objects/nodes to the scene using custom derived `Scene::Prefab` objects. Objects can also be added directly to the scene, which is stored in `main_scene_`.
3. Set `initialized = true` at the end of setup

Once defined your entire main loop should be as simple as this:
```c++
int main(int /*argc*/, char** argv) {
    CustomGameApplication game{argv};  // argv is passed in so resource managers know where to find models, textrures, and shader files.
    game.setup();
    return game.loop();
}
```

#### Scene::Scene
The Scene object owns and orchestrates the objects that make up the game. Objects can be added to the scene one by one or in groups using Scene::Prefab. Once added, objects can be referenced and retrieved from the scene using their auto-generated ids.

While objects maintain a hierarchical relationship to one another, the scene actually maintains them in a flat map data structure. All scene objects are stored as unique pointers to the `Node` base class. Special scene behavior can be triggered by assigning `SceneProperties` to the derived class.

Here is an example of basic scene+object interaction:
```c++
auto root_id = scene.create_object<Node>();  // Simple objects can be created by the scene itself using a template function that accepts a `Node` or derived class.

// Alternatively objects can be created outside the scene and moved in.
// Note that the `root_id` of the previously created node is passed in as an optional argument.
// This makes `cube` a child of the root node. More on that in the `Objects` section.
auto object_id = scene.add_scene_object(std::make_unique<GameObject>("suzanne.gltf", "default"), root_id);

// Currently scenes lack a default global illumination so we must pass in a light source to illuminate our scene.
auto light_id = scene.add_scene_object(std::make_unique<LightSource>("sphere.gltf", Vector3(1), 0.4), root_id);
// Once created we can fetch a reference to the object and edit its parameters via chained methods.
scene.get_scene_object(light_id).set_position(5, 5, 5).set_scale(.25, .25, .25);

// A camera is also required to render the scene.
// Note that a camera can be added to the hierarchy just like any other node.
// This allows us to move or rotate the `camera_anchor` node and have the `camera` move with it,
// maintaining its relative offset+rotation
auto camera_anchor_id = scene.create_object<Node>();
auto camera_id = scene.add_scene_object(std::make_unique<Camera>(
65.0, aspect_ratio, 0.1, 100.0), camera_anchor_id);
scene.get_scene_object(camera_id).set_position(5, 5, 5).look_at({0,0,0});
```

#### Scene::Prefab
Creating complex scene hierarchies quickly results in a lot of boilerplate. To make this a little easier there is the `Prefab` struct.

Prefabs are designed to contain code snippets like the one above that result in the creation of portion of a scene tree.

A user creates a prefab by inheriting from the base object and overriding the `initialize(Scene::Scene &scene)` method. All the node creation and scene addition code should be handled in the function body. The root id of the newly created subtree should be returned.

Both the editor and the demo game contain examples of how prefabs might work and how they are integrated into an Application.

#### Objects
Game logic, visual components, and (eventually) physics are all encapsulated in Objects. Objects are inspired by Godot's Nodes and use a combination of inheritance, composition, and hierarchical nesting to achieve reusable game logic.

This is the current object taxonomy:

- `Node`: the base class, describes a point in 3D space with position, rotation, and scale. It can optionally have a controller (see below) assigned to it.
  - `Camera`: a node with methods to generate projection and view matrices based on the camera's position in the scene. It can be moved just like any other Node.
  - `RenderedObject`: a base class for any node that will be rendered on screen, meaning it contains a model (mesh + optional textures) and shader (vertex + fragment)
    - `GameObject`: a flexible implementation of the above (which will almost certainly be renamed at some point)
    - `LightSource`: a special implementation of a rendered object that provides lighting information to the scene

Objects can live in hierarchical trees. Each object can optionally have one parent and unlimited children.

Objects maintain their own _local_ position, rotation, and scale information—together referred to as a transform. They also have a _global_ transform that is derived from their parents' transform (if they have a parent) _plus_ their own. By cascading this information down from parent to descendents, complex scenes can be manipulated with ease.

Game logic lives in the `update()` method. This method is called every frame by the scene object with the `delta time` since the last frame passed in as an argument. It's within this loop that user inputs can be polled, transform information can be edited, and the scene can be interacted with.

Users can create custom objects by inheriting from the appropriate Object class and overriding the `update()` method with custom logic. Alternatively (or in addition), object behavior can be augmented by assigning a custom controller. Controllers are called during the update loop so they're useful for creating reusable behavioral code. See the example `ShipController` class in the demo game.

#### Resources
Resources are relatively memory-heavy entities that can be shared across many Objects. Currently this includes: textures, shaders, and models (which contain a mesh and optionally a texture).

Resources are managed by a ResourceManager singleton that can be accessed via `Manager::manager_name::get("relative_path_to_resource")`. For example:
```c++
Model* my_model = Manager::model_manager::get("model/my_model.gltf");
```
The managers doll out raw pointers. They maintain a crude form of ref-counting so that unused resourced can be unloaded when no longer reference by any object.

To that end, any Object that contains a resource pointer **must** call the `release()` method on the manager singleton. Not doing so will result in memory leaks.

#### Singletons
There are currently two singletons of note in the project. Both need to be initialized at the beginning of any game. Currently this is handled in the `Application` constructor.

- `Managers`: discussed above
- `Input`: provides user input information. It must be polled once a frame, which is handled automatically by the `Application` loop. Currently it has very limited functionality.

#### Vector Math
I've implemented custom classes for vectors—`Vector2`, `Vector3`, and the not-very-useful `Vector4`—as well as transformation matrices (`Transform`) and quaternions (`Quaternion`).

Transforms are 4x4 matrices that are stored in row-major order. There are helper methods to apply rotation, translation, and scaling as well as create perspective and view matrices. Matrix values can be set directly with the `at(row, column)` method if you know what you're doing. While rotation can be achieved using Euler angles directly, I'd advise using the Quaternions and the `slerp` method to avoid gimble lock and other weird things I don't really understand about 3D rotation.

## Acknowledgements
I owe considerable credit to [LearnOpenGL](https://learnopengl.com/) for its incredible walkthrough of rendering fundamentals.

In addition, I've taken a good deal of inspiration from the [Godot](https://godotengine.org/) engine and its source code, particularly related to vector/matrix math.

# TODOs

- Graphics
  - [ ] Directional, spot, and point light types
  - [ ] Multi-light rendering
  - [ ] Basic particle system
  - [ ] Environmental attributes (fog, etc.)

- Objects
  - [x] Scene object
  - [x] Hierarchical scene/node graph
  - [ ] Frustum culling (using spherical or AABB volumes)
  - [ ] Basic scene serialization

- Physics
  - [ ] Basic physics bodies: static, ridged, and character bodies
  - [ ] Basic physical shapes: sphere, capsule, rect
  - [ ] Force integration
  - [ ] Collisions

- Stretch Goals
  - [ ] 3D editor (use ImGUI)
  - [ ] UI elements
  - [ ] PBR
  - [ ] Shadows
  - [ ] Audio
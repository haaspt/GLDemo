# Overview

This project is my attempt to learn the priciples of 3D game development by building an engine from the ground up. It also represents my first proper foray into C++, so it's almost certainly littered with bad practices and wonky code.

## Acknowledgements
I owe considerable credit to [LearnOpenGL](https://learnopengl.com/) for it's incredible walkthrough of rendering fundamentals.

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
  - [ ] Frustrum culling (using spherical or AABB volumes)
  - [ ] Basic scene serialization

- Physics
  - [ ] Basic physics bodies: static, ridgid, and character bodies
  - [ ] Basic physical shapes: sphere, capsule, rect
  - [ ] Force integration
  - [ ] Collisions

- Stretch Goals
  - [ ] 3D editor (use ImGUI)
  - [ ] UI elements
  - [ ] PBR
  - [ ] Shadows
  - [ ] Audio
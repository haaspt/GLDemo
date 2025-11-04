#pragma once

class Camera {
public:
    Camera(glm::mat4 view, glm::mat4 projection)
        : view(view), projection(projection) {}
    glm::mat4 view;
    glm::mat4 projection;
};
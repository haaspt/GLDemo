#pragma once
#include <glm/glm.hpp>

class Camera {
public:
    Camera(float fov_deg, float aspect_ratio, float near_plane_dist, float far_plane_dist) {
        projection = glm::perspective(
                glm::radians(fov_deg),
                aspect_ratio,
                near_plane_dist,
                far_plane_dist
            );
    }
    
    glm::vec3 get_position() const {
        return position;
    }
    void set_position(glm::vec3 pos) {
        position = pos;
        view_dirty = true;
    }
    void set_position(float x, float y, float z) {
        set_position(glm::vec3(x, y, z));
    }

    float get_pitch() const {
        return glm::degrees(pitch);
    }
    void set_pitch(float pitch_deg) {
        pitch = glm::radians(pitch_deg);
        view_dirty = true;
    }

    float get_yaw() const {
        return glm::degrees(yaw);
    }
    void set_yaw(float yaw_deg) {
        yaw = glm::radians(yaw_deg);
        view_dirty = true;
    }

    const glm::mat4& get_view_matrix() const {
        if (view_dirty) {
            update_view_matrix();
        }
        return view;
    }
    const glm::mat4& get_projection_matrix() const {
        return projection;
    }

private:
    mutable bool view_dirty = true;
    // TODO: allow FOV and aspect changes
    
    glm::vec3 position = glm::vec3(0.0f);
    float yaw = 0.0f;
    float pitch = 0.0f;
    
    mutable glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection;

    void update_view_matrix() const {
        glm::vec3 front;
        front.x = cos(yaw) * cos(pitch);
        front.y = sin(pitch);
        front.z = sin(yaw) * cos(pitch);
        front = glm::normalize(front);
        
        view = glm::lookAt(
            position,
            position + front,
            glm::vec3(0.0f, 1.0f, 0.0f)  // UP
        );
        
        view_dirty = false;
    }
};

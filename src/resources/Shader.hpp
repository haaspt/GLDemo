#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>
#include <OpenGL/gl3.h>


class Shader {
private:
    mutable std::unordered_map<std::string, GLuint> uniform_id_lookup_;
public:
    unsigned int id;
    bool is_valid = true;

    Shader(const std::string& vertex_shader_path, const std::string& fragment_shader_path);

    ~Shader();

    Shader(const Shader&) = delete;

    Shader& operator=(const Shader&) = delete;

    Shader(Shader&&) = delete;

    Shader& operator=(Shader&&) = delete;

    void use() const;

    void set_bool(const std::string& uniform_name, bool value) const;

    void set_int(const std::string& uniform_name, int value) const;

    void set_float(const std::string& uniform_name, float value) const;

    void set_vec3(const std::string& uniform_name, const glm::vec3& value) const;

    void set_mat4(const std::string& uniform_name, const glm::mat4& value) const;

    GLuint get_uniform_location(const std::string& uniform_name) const;
};
#pragma once

#include <string>

#include <glm/glm.hpp>


class Shader {
public:
    unsigned int id;
    bool is_valid = true;

    Shader(const std::string& vertex_shader_path, const std::string& fragemnt_shader_path);

    ~Shader();

    void use() const;

    void set_bool(const std::string& param_name, bool value) const;

    void set_int(const std::string& param_name, int value) const;

    void set_float(const std::string& param_name, float value) const;

    void set_vec3(const std::string& param_name, const glm::vec3& value) const;

    void set_mat4(const std::string& param_name, const glm::mat4& value) const;
};

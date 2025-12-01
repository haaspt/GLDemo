#include <fstream>
#include <sstream>
#include <iostream>

#include <OpenGL/gl3.h>
#include <glm/gtc/type_ptr.hpp>

#include "resources/Shader.hpp"


std::string load_shader_source_from_file(const std::string& shader_path) {
    std::string shader_code;
    std::ifstream shader_file;

    // enable exceptions
    shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        shader_file.open(shader_path);
        std::stringstream shader_stream;
        shader_stream << shader_file.rdbuf();
        shader_file.close();
        shader_code = shader_stream.str();
    } catch (std::ifstream::failure e) {
        std::cerr << e.what() << '\n';
    }

    return shader_code;
}

Shader::Shader(const std::string& vertex_shader_path, const std::string& fragment_shader_path) {
    const std::string v_shader_source = load_shader_source_from_file(vertex_shader_path);
    const std::string f_shader_source = load_shader_source_from_file(fragment_shader_path);

    unsigned int v_shader, f_shader;
    int success;
    char info_log[512];

    v_shader = glCreateShader(GL_VERTEX_SHADER);
    f_shader = glCreateShader(GL_FRAGMENT_SHADER);

    const char* v_src = v_shader_source.c_str();

    glShaderSource(
        v_shader,
        1,
        &v_src,
        0
    );

    glCompileShader(v_shader);


    glGetShaderiv(v_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(v_shader, 512, 0, info_log);
        printf("ERROR — Vertex shader failed to compile with error: %s\n", info_log);
        is_valid = false;
        id = 0;
        return;
    }

    const char* f_src = f_shader_source.c_str();

    glShaderSource(
        f_shader,
        1,
        &f_src,
        0
    );

    glCompileShader(f_shader);

    glGetShaderiv(f_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(f_shader, 512, 0, info_log);
        printf("ERROR — Fragment shader failed to compile with error: %s\n", info_log);
        is_valid = false;
        id = 0;
        glDeleteShader(v_shader);
        return;
    }

    // Link program

    id = glCreateProgram();

    glAttachShader(id, v_shader);
    glAttachShader(id, f_shader);

    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 512, 0, info_log);
        printf("ERROR — Shader program failed to link with error: %s\n", info_log);
        is_valid = false;
        id = 0;
    }

    glDeleteShader(v_shader);
    glDeleteShader(f_shader);
}

Shader::~Shader() {
    if (id != 0) glDeleteProgram(id);
}

void Shader::use() const {
    glUseProgram(id);
}

void Shader::set_bool(const std::string& uniform_name, bool value) const {
    glUniform1i(
        get_uniform_location(uniform_name),
        (int) value
    );
}

void Shader::set_int(const std::string& uniform_name, int value) const {
    glUniform1i(
        get_uniform_location(uniform_name),
        value
    );
}

void Shader::set_float(const std::string& uniform_name, float value) const {
    glUniform1f(
        get_uniform_location(uniform_name),
        value
    );
}

void Shader::set_vec3(const std::string& uniform_name, const glm::vec3& value) const {
    glUniform3fv(
        get_uniform_location(uniform_name),
        1,
        glm::value_ptr(value)
    );
}

void Shader::set_mat4(const std::string& uniform_name, const glm::mat4& value) const {
    glUniformMatrix4fv(
        get_uniform_location(uniform_name),
        1,
        GL_FALSE,
        glm::value_ptr(value)
    );
}

GLuint Shader::get_uniform_location(const std::string& uniform_name) const {
    if (uniform_id_lookup_.find(uniform_name) != uniform_id_lookup_.end()) {
        return uniform_id_lookup_[uniform_name];
    }
    GLuint loc = glGetUniformLocation(id, uniform_name.c_str());
    uniform_id_lookup_[uniform_name] = loc;
    return loc;
}

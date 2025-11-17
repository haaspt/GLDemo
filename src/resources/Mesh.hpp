#pragma once

#include <vector>

#include <OpenGl/gl3.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Mesh
{
private:
    struct MeshData {
        const std::vector<float> vertices;
        const std::vector<int> indices;
        const std::string name;
    };
    MeshData mesh_data;
    GLuint VAO, VBO, EBO;
    unsigned int index_count;
    MeshData get_mesh_data(const json& j_data) const ;
    void gl_init();
public:
    explicit Mesh(const json& json_data);
    ~Mesh();
    
    void draw() const;
};

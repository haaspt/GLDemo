#pragma once
#include <vector>
#include <OpenGl/gl3.h>
#include "Shader.hpp"

class Mesh
{
public:
    unsigned int id;

    Mesh(std::vector<float> verts, std::vector<int> idxs);
    ~Mesh();
    
    void draw() const;
private:
    GLuint VAO, VBO, EBO;
    unsigned int index_count;
};

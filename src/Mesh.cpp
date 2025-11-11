#include "Mesh.hpp"

Mesh::Mesh(const std::vector<float>& verts, const std::vector<int>& idxs)
    : index_count(idxs.size())
{
    // VAO setup
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO/EBO setup
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        verts.size() * sizeof(float),
        verts.data(),
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        index_count * sizeof(int),
        idxs.data(),
        GL_STATIC_DRAW
    );

    // Position attribute
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)0  // Starting at index 0
    );
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)(3*sizeof(float))  // offset by 3 preceding pos floats
    );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);  // unset VAO
}


Mesh::~Mesh() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}


void Mesh::draw() const {
    glBindVertexArray(VAO);
    glDrawElements(
        GL_TRIANGLES,
        index_count,
        GL_UNSIGNED_INT,
        0
    );
}

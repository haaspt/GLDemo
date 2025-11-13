#include "Mesh.hpp"

Mesh::MeshData Mesh::get_mesh_data(const json& j_data) const {
    MeshData new_data{
        j_data["vertices"].get<std::vector<float>>(),
        j_data["indices"].get<std::vector<int>>(),
        j_data["name"]
        };
    return new_data;
}

Mesh::Mesh(const json& json_data) :
    mesh_data(get_mesh_data(json_data)), index_count(mesh_data.indices.size()) {
    gl_init();
}

void Mesh::gl_init() {
    // VAO setup
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO/EBO setup
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        mesh_data.vertices.size() * sizeof(float),
        mesh_data.vertices.data(),
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        index_count * sizeof(int),
        mesh_data.indices.data(),
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

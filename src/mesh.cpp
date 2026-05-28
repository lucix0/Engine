#include "mesh.hpp"

#include "shader.hpp"

#include <glad/glad.h>

#include <bgfx/bgfx.h>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setupMesh();
}

void Mesh::Draw(Shader& shader) {
    unsigned int diffuseNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = textures[i].type;
        if (name != "texture_diffuse")
            continue;

        number = std::to_string(diffuseNr++);
        
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    // glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::setupMesh() {
    bgfx::VertexLayout layout;
    layout
        .begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 2, bgfx::AttribType::Float)
        .end();

    this->VBO = bgfx::createVertexBuffer(
        bgfx::makeRef(this->vertices.data(), 32 * this->vertices.size()),
        layout
    );

    this->EBO = bgfx::createIndexBuffer(
        bgfx::makeRef(this->indices.data(), 2 * this->indices.size())
    );
}

#include "mesh.hpp"
#include "shader.hpp"

#include <glad/glad.h>
#include <bgfx/bgfx.h>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
	this->vertices = vertices;
	this->indices = indices;

	setupMesh();
}

void Mesh::Draw(Shader& shader) {
    bgfx::setVertexBuffer(0, VBO);
    bgfx::setIndexBuffer(EBO);
    bgfx::submit(0, shader.ID);
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

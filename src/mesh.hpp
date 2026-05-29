#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <bgfx/bgfx.h>			
#include "shader.hpp"

struct Vertex {
	glm::vec3 Position;
};

class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	void Draw(Shader& shader);
private:
	bgfx::VertexBufferHandle VBO;
	bgfx::IndexBufferHandle EBO;

	void setupMesh();
};


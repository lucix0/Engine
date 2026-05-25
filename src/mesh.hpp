#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <bgfx/bgfx.h>			
#include "shader.hpp"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(Shader& shader);
private:
	bgfx::VertexBufferHandle VBO;
	bgfx::IndexBufferHandle EBO;

	void setupMesh();
};


#pragma once
#include <string>
#include <vector>
#include <assimp/mesh.h>
#include <assimp/scene.h>

#include "mesh.hpp"
#include "shader.hpp"

class Model {
public:
	Model(const char* path);
	void Draw(Shader& shader);
private:
	std::vector<Mesh> meshes;
	std::string directory;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
};
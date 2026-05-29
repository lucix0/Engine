#include "shader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	char* vMem;
	char* fMem;
	unsigned int vSize, fSize;

	try {
		vShaderFile.open(vertexPath, std::ios::binary | std::ios::in);
		fShaderFile.open(fragmentPath, std::ios::binary | std::ios::in);

		vSize = vShaderFile.tellg();
		fSize = fShaderFile.tellg();
		vMem = new char[vSize];
		fMem = new char[fSize];
		vShaderFile.read(vMem, vSize);
		fShaderFile.read(fMem, fSize);

		vShaderFile.close();
		fShaderFile.close();
	}
	catch(std::ifstream::failure e) {
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}

	bgfx::ShaderHandle vertex, fragment;
	vertex = bgfx::createShader(bgfx::makeRef(vMem, vSize));
	fragment = bgfx::createShader(bgfx::makeRef(fMem, fSize));

	ID = bgfx::createProgram(vertex, fragment, true);

	delete[] vMem;
	delete[] fMem;
}
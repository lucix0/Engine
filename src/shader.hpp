#pragma once
#include <string>
#include <bgfx/bgfx.h>
#include <glm/glm.hpp>

class Shader {
public:
	bgfx::ProgramHandle ID;

	Shader(const std::string &vertexPath, const std::string &fragmentPath);
};
#pragma once
#include <string>

#include "bgfx/bgfx.h"

bgfx::ShaderHandle loadShader(const std::string& shaderPath);

bgfx::ProgramHandle loadProgram(const std::string& vsPath, const std::string& fsPath);
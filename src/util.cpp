#include "util.hpp"

#include <fstream>
#include <iostream>

bgfx::ShaderHandle loadShader(const std::string& shaderPath) {
    std::ifstream shaderFile;
    shaderFile.open(shaderPath, std::ios::in | std::ios::binary | std::ios::ate);

    if (!shaderFile.is_open()) {
        std::cerr << "Error opening file " << shaderPath << std::endl;
    }

    std::streampos fileSize = shaderFile.tellg();
    auto* memblock = new char[fileSize];
    shaderFile.seekg(0, std::ios::beg);
    shaderFile.read(memblock, fileSize);
    shaderFile.close();

    const bgfx::ShaderHandle sh = bgfx::createShader(bgfx::copy(memblock, fileSize));

    delete[] memblock;

    return sh;
}

bgfx::ProgramHandle loadProgram(const std::string& vsPath, const std::string& fsPath) {
    const auto vsh = loadShader(vsPath);
    const auto fsh = loadShader(fsPath);
    auto program = bgfx::createProgram(vsh, fsh);

    bgfx::destroy(vsh);
    bgfx::destroy(fsh);

    return program;
}

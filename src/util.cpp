#include "util.h"

#include <fstream>
#include <iostream>

bgfx::ShaderHandle loadShader(const std::string& shaderName) {
    // Location of shaders.
    std::string path = "shaders/bin/";

    // Choose subfolder depending on renderer.
    switch (bgfx::getRendererType()) {
        case bgfx::RendererType::Noop:
        case bgfx::RendererType::Direct3D11:
        case bgfx::RendererType::Direct3D12: path.append("dx11/");  break;
        case bgfx::RendererType::Agc:
        case bgfx::RendererType::Gnm:        path.append("pssl/");  break;
        case bgfx::RendererType::Metal:      path.append("metal/"); break;
        case bgfx::RendererType::Nvn:        path.append("nvn/");   break;
        case bgfx::RendererType::OpenGL:     path.append("glsl/");  break;
        case bgfx::RendererType::OpenGLES:   path.append("essl/");  break;
        case bgfx::RendererType::Vulkan:     path.append("spirv/"); break;

        case bgfx::RendererType::Count:
            std::cerr << "Error: Unknown Renderer" << std::endl;
            break;
    }

    // Finally, append the specific shader name.
    path.append(shaderName);

    std::ifstream shaderFile;
    shaderFile.open(path, std::ios::in | std::ios::binary | std::ios::ate);

    if (!shaderFile.is_open()) {
        std::cerr << "Error opening file " << path << std::endl;
    }

    const std::streampos fileSize = shaderFile.tellg();
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
    const auto program = bgfx::createProgram(vsh, fsh);

    bgfx::destroy(vsh);
    bgfx::destroy(fsh);

    return program;
}

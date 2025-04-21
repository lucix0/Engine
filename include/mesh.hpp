#pragma once
#include <vector>
#include <array>

struct Vertex {
    float x, y, z;
    uint32_t color;
};

struct Mesh {
    std::vector<Vertex> vertexList;
    std::vector<uint16_t> indexList;
};

struct Model {
    std::vector<Mesh> meshes;
    std::array<float, 16> modelMatrix;

    // Purpose: Load a model from the filesystem.
    void loadFromFile(const std::string& filename);
};
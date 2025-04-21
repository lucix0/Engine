#include <iostream>
#include <mesh.hpp>

#include <string>

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

void Model::loadFromFile(const std::string& filename) {
    std::string path = "resources/meshes/" + filename;

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_SortByPType
    );

    if (nullptr == scene) {
        std::cerr << importer.GetErrorString() << std::endl;
    }



    for (int m = 0; m < scene->mNumMeshes; m++) {
        auto newMesh = Mesh {
            std::vector<Vertex>(),
            std::vector<uint16_t>(),
        };

        for (int i = 0; i < scene->mMeshes[m]->mNumFaces; i++) {
            for (int j = 0; j < scene->mMeshes[m]->mFaces[i].mNumIndices; j++) {
                newMesh.indexList.push_back(scene->mMeshes[m]->mFaces[i].mIndices[j]);
            }
        }

        for (int i = 0; i < scene->mMeshes[m]->mNumVertices; i++) {
            Vertex vertex = {
                scene->mMeshes[m]->mVertices[i].x,
                scene->mMeshes[m]->mVertices[i].y,
                scene->mMeshes[m]->mVertices[i].z,
                0xff32a852,
            };

            newMesh.vertexList.push_back(vertex);
        }

        meshes.push_back(newMesh);
    }
}

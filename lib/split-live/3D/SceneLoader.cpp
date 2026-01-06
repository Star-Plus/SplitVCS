//
// Created by lightum on 1/6/26.
//

#include "SceneLoader.h"

#include <stdexcept>

#include "../../../cmake-build-debug/_deps/assimp-src/code/AssetLib/STEPParser/STEPFileReader.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

namespace Split {

    const aiScene* Load(const char* filename) {
        Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile(
            filename,
            aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenNormals
        );

        if (!scene || !scene->HasMeshes()) {
            throw std::runtime_error("Failed to load mesh");
        }

        return scene;
    }

}

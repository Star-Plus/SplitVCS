//
// Created by lightum on 1/6/26.
//

#include "MeshAdapter.h"

namespace Split {

    std::unique_ptr<draco::Mesh> convertToDraco(const aiMesh* src) {
        auto mesh = std::make_unique<draco::Mesh>();

        mesh->set_num_points(src->mNumVertices);

        return mesh;
    }

}
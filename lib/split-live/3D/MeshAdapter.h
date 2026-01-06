//
// Created by lightum on 1/6/26.
//

#ifndef SPLITVCS_MESHADAPTER_H
#define SPLITVCS_MESHADAPTER_H

#include "draco/mesh/mesh.h"
#include "assimp/scene.h"
#include <memory>

namespace Split {
    class MeshAdapter {
    public:
        static std::unique_ptr<draco::Mesh> convertToDraco(const aiMesh* src);

    };
}
#endif //SPLITVCS_MESHADAPTER_H
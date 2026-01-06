//
// Created by lightum on 1/6/26.
//

#ifndef SPLITVCS_SCENELOADER_H
#define SPLITVCS_SCENELOADER_H
#include "assimp/scene.h"

namespace Split {
    class SceneLoader {
    public:
        static const aiScene* Load(const char* filename);
    };
}
#endif //SPLITVCS_SCENELOADER_H
//
// Created by Ahmed Mustafa on 1/24/2026.
//

#ifndef SPLITVCS_ASSET_H
#define SPLITVCS_ASSET_H

#include <string>

#include "enums/AssetType.h"

namespace Split
{

    struct Asset
    {
        std::string path;
        AssetType type;
    };

}

#endif //SPLITVCS_ASSET_H
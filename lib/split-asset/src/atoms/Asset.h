//
// Created by Ahmed Mustafa on 1/24/2026.
//

#ifndef SPLITVCS_ASSET_H
#define SPLITVCS_ASSET_H

#include <string>

#include "enums/EncodeType.h"

namespace Split
{

    struct Asset
    {
        std::string path;
        EncodeType type;
    };

}

#endif //SPLITVCS_ASSET_H
//
// Created by Ahmed Mustafa on 1/22/2026.
//

#ifndef SPLITVCS_FSMETADATA_H
#define SPLITVCS_FSMETADATA_H
#include <fstream>

#include "core/Alias.h"

namespace Split
{

    namespace FsMetadata
    {

        size_t calculateFileSize(const str& filepath);

    }

}


#endif //SPLITVCS_FSMETADATA_H
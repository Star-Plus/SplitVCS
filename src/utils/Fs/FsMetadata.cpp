//
// Created by Ahmed Mustafa on 1/22/2026.
//

#include "FsMetadata.h"

#include <filesystem>

namespace Split
{

    namespace FsMetadata
    {
        size_t calculateFileSize(const str& file)
        {
            return std::filesystem::file_size(file);
        }
    }

}

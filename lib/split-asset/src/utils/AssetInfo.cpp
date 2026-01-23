#include "string"
#include "AssetInfo.h"

namespace Split {

    namespace AssetInfo {

        bool isImage(const std::string& extension) {
            return extension == ".png" || extension == ".jpg" || extension == ".jpeg" ||
                   extension == ".bmp" || extension == ".tiff" || extension == ".webp";
        }

    }

}
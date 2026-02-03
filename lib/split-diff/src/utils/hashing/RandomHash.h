//
// Created by Ahmed Mustafa on 2/2/2026.
//

#ifndef SPLITVCS_RANDOMHASH_H
#define SPLITVCS_RANDOMHASH_H

#include <string>

namespace Split
{
    class RandomHash
    {
    public:
        static std::string generateHash(size_t length);
    };
} // Split

#endif //SPLITVCS_RANDOMHASH_H
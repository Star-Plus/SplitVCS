//
// Created by Ahmed Mustafa on 8/26/2025.
//

#ifndef PACKNEGOTIATION_H
#define PACKNEGOTIATION_H
#include <components/Repository.h>

namespace Split {

    class PackNegotiation {

        Repository repo;

    public:
        explicit PackNegotiation(const str& repoPath) : repo(repoPath) {}

        str negotiatePack(const str& requestedCommit) const;

    };

}



#endif //PACKNEGOTIATION_H

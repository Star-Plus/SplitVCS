#include "core/Alias.h"

namespace Split {

    class ManualCommiter {

        const str& repoPath;

    public:
        ManualCommiter(const str& repoPath);
        ~ManualCommiter();

        void uploadTree(const str& treeHash, const str& treeContent);
        void uploadCommit(const str& commitHash, const str& commitContent);
    };

}
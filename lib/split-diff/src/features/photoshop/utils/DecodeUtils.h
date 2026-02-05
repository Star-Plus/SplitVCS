//
// Created by Ahmed Mustafa on 2/4/2026.
//

#ifndef SPLITVCS_DECODEUTILS_H
#define SPLITVCS_DECODEUTILS_H

#include <stack>
#include <queue>

#include "utils/path/PathUtils.h"

namespace Split::DecodeUtils
{
    template <typename T>
    std::stack<T> copyStackEditedWithSuffix(std::stack<T> s1, const T& suffix)
    {
        std::queue<T> tempQueue;

        while (!s1.empty())
        {
            tempQueue.push(s1.top()+suffix);
            s1.pop();
        }

        std::stack<T> stack;
        while (!tempQueue.empty())
        {
            stack.push(tempQueue.front());
            tempQueue.pop();
        }

        return stack;
    }

    inline std::stack<std::string> copyStackSuffixEdited(std::stack<std::string> s1, const std::string& suffix, const std::string& suffixToRemove = "")
    {
        std::queue<std::string> tempQueue;

        while (!s1.empty())
        {
            auto top = s1.top();
            PathUtils::removeSuffix(top, suffixToRemove);
            tempQueue.push(top+suffix);
            s1.pop();
        }

        std::stack<std::string> stack;
        while (!tempQueue.empty())
        {
            stack.push(tempQueue.front());
            tempQueue.pop();
        }

        return stack;
    }

    inline void extractDeltaArchives(std::stack<std::string> deltas, const Bit7Archive& psdArchive)
    {
        while (!deltas.empty())
        {
            const auto& delta = deltas.top();

            auto exportPath = delta;
            PathUtils::removeSuffix(exportPath, ".7z");
            psdArchive.ExtractArchive(delta, exportPath + "/");
            deltas.pop();
        }
    }
}

#endif //SPLITVCS_DECODEUTILS_H
//
// Created by Ahmed Mustafa on 2/4/2026.
//

#ifndef SPLITVCS_DECODEUTILS_H
#define SPLITVCS_DECODEUTILS_H

#include <stack>
#include <queue>

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
}

#endif //SPLITVCS_DECODEUTILS_H
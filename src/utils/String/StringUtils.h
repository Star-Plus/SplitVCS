//
// Created by Ahmed Mustafa on 1/22/2026.
//

#ifndef SPLITVCS_STRINGUTILS_H
#define SPLITVCS_STRINGUTILS_H

#include <vector>
#include <string>

namespace StringUtils
{
    inline std::vector<std::string> split(const std::string& input, const std::string& delimiter)
    {
        std::vector<std::string> tokens;

        std::string token;

        for (auto i = 0; i != input.length(); ++i)
        {
            bool tokenFound = false;

            for (auto j = 0; j != delimiter.length(); ++j)
            {
                if (input[i] == delimiter[j])
                {
                    tokens.push_back(token);
                    token.clear();
                    tokenFound = true;
                    break;
                }
            }

            if (!tokenFound)
            {
                token += input[i];
            }
        }

        if (!token.empty())
            tokens.push_back(token);

        return tokens;
    }

    inline std::string concat(const std::vector<std::string>& tokens, const std::string& delimiter)
    {
        std::string result;
        for (const auto& token : tokens)
        {
            result += token + delimiter;
        }

        return result;
    }
}

#endif //SPLITVCS_STRINGUTILS_H
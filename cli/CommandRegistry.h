//
// Created by Ahmed Mustafa on 1/23/2026.
//

#ifndef SPLITVCS_COMMANDREGISTERY_H
#define SPLITVCS_COMMANDREGISTERY_H
#include <memory>
#include <unordered_map>

#include "Command.h"

namespace Split
{
    class CommandRegistry
    {
    public:
        static CommandRegistry& instance();

        void registerCommand(std::unique_ptr<Command> command);
        Command* getCommand(const std::string& name);
        std::vector<Command*> all() const;

    private:
        std::unordered_map<str, std::unique_ptr<Command>> mCommands;
    };
} // Split

#endif //SPLITVCS_COMMANDREGISTERY_H
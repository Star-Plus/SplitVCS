//
// Created by Ahmed Mustafa on 1/23/2026.
//

#include "CommandRegistry.h"

#include <iostream>
#include <ostream>

namespace Split
{

    CommandRegistry& CommandRegistry::instance()
    {
        static CommandRegistry instance;
        return instance;
    }

    void CommandRegistry::registerCommand(std::unique_ptr<Command> command)
    {
        mCommands[command->name()] = std::move(command);
    }

    Command* CommandRegistry::getCommand(const std::string& name)
    {
        const auto it = mCommands.find(name);
        if (it == mCommands.end()) return nullptr;

        return it->second.get();
    }

    std::vector<Command*> CommandRegistry::all() const
    {
        std::vector<Command*> result;
        for (auto& [_, cmd] : mCommands)
        {
            result.push_back(cmd.get());
        }

        return result;
    }

} // Split
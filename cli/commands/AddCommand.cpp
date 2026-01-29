//
// Created by Ahmed Mustafa on 1/23/2026.
//

#include <filesystem>

#include "SplitInterface.h"
#include "../Command.h"
#include "../CommandRegistry.h"

namespace Split
{
    class AddCommand : public Command
    {
    public:
        str name() const override { return "add"; }
        str description() const override { return "Stages a file."; }
        str usage() const override { return "usage: add [options]"; }

        int run(const std::vector<str>& args) override
        {
            const str repoPath = args.empty() ? std::filesystem::current_path().string() : args.at(0);
            SplitInterface::Add(repoPath, args[1]);
            return 0;
        }
    };
}

static bool registered = []
{
    Split::CommandRegistry::instance().registerCommand(
        std::make_unique<Split::AddCommand>()
    );

    return true;
}();

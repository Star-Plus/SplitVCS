//
// Created by Ahmed Mustafa on 1/23/2026.
//

#include <filesystem>

#include "SplitInterface.h"
#include "../Command.h"
#include "../CommandRegistry.h"

namespace Split
{
    class InitCommand : public Command
    {
    public:
        str name() const override { return "init"; }
        str description() const override { return "Initialize a new split repository."; }
        str usage() const override { return "usage: init [options]"; }

        int run(const std::vector<str>& args) override
        {
            const str repoPath = args.empty() ? std::filesystem::current_path().string() : args.at(0);
            SplitInterface::Init(repoPath);
            return 0;
        }
    };
}

static bool registered = []
{
    Split::CommandRegistry::instance().registerCommand(
        std::make_unique<Split::InitCommand>()
    );

    return true;
}();

//
// Created by Ahmed Mustafa on 1/23/2026.
//

#include <iostream>

#include "SplitInterface.h"
#include "../Command.h"
#include "../CommandRegistry.h"

namespace Split
{

    class TinyAssetCommand : public Command
    {
    public:
        str name() const override { return "tiny-asset"; }
        str description() const override { return "TinyAsset"; }
        str usage() const override { return "tiny-asset [repo path, asset hash, quality]"; }

        int run(const std::vector<str>& args) override
        {
            try
            {
                const int quality = std::stoi(args.at(2));
                const auto assetPath = SplitInterface::GetTinyAsset(args[0], args[1], quality);
                std::cout << assetPath << std::endl;
                return 0;
            } catch (std::exception& e)
            {
                std::cerr << "TinyAsset error " << e.what() << std::endl;
                return 1;
            }

        }
    };

}

static bool registered = []
{
    Split::CommandRegistry::instance().registerCommand(
        std::make_unique<Split::TinyAssetCommand>()
    );

    return true;
}();
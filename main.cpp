#include <iostream>
#include <ostream>

#include "cli/CommandRegistry.h"
#include "cli/Parser.h"

int main(int argc, char **argv) {

    const auto arguments = Split::Parser::Parse(argc, argv);

    Split::Command* cmd = Split::CommandRegistry::instance().getCommand(arguments.first);

    const auto allCommands = Split::CommandRegistry::instance().all();

    if (cmd == nullptr)
    {
        std::cerr << "Unknown command." << std::endl;
        return 1;
    }

    return cmd->run(arguments.second);
}

#pragma once

#include <CLI/CLI.hpp>

class ICommand {
public:
    virtual ~ICommand() = default;
    
    // Every command must register its subcommand and options to the main CLI app
    virtual void register_command(CLI::App& app) = 0;
};
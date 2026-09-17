#include <iostream>
#include <vector>
#include <memory>
#include <CLI/CLI.hpp>
#include <dotenv.h>

#include "commands/command.hpp"
#include "commands/seeder.hpp"

int main(int argc, char* argv[]) {
    CLI::App app{"C++ Production Web Application CLI"};

    std::string env_file = ".env";
    app.add_option("-e,--env", env_file, "Path to .env file")->default_str(".env");

    std::vector<std::unique_ptr<ICommand>> commands;
    commands.push_back(std::make_unique<SeederCommand>());
    // commands.push_back(std::make_unique<ServeCommand>());

    for (const auto& cmd : commands) {
        cmd->register_command(app);
    }

    if (argc == 1) {
        std::cout << app.help() << "\n";
        return 0;
    }

    try {
        app.parse(argc, argv);
    } 
    catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    dotenv::init(env_file.c_str());

    return 0;
}
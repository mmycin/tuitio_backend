#include <iostream>
#include <CLI/CLI.hpp>
#include <dotenv.h>
#include <spdlog/spdlog.h>
#include "commands/seeder.hpp"

int main(int argc, char* argv[]) {
    CLI::App app{"C++ Production Web Application CLI"};

    // Global option for environment file path
    std::string env_file = ".env";
    app.add_option("-e,--env", env_file, "Path to .env file")->default_str(".env");

    // Subcommand: seed
    auto* seed_cmd = app.add_subcommand("seed", "Run a database seeder script");
    std::string seeder_file;
    seed_cmd->add_option("-f,--file", seeder_file, "Path to seeder SQL file")->required();

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    // Initialize environment variables globally based on parsed flag
    dotenv::init(env_file.c_str());

    // Execute logic based on active subcommand
    if (app.got_subcommand(seed_cmd)) {
        return run_seeder(seeder_file);
    } 

    else {
        std::cout << app.help() << "\n";
        return 0;
    }
}
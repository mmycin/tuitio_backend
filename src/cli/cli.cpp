#include <CLI/CLI.hpp>
#include <dotenv.h>
#include <memory>
#include <spdlog/spdlog.h>
#include <vector>


#include "commands/command.hpp"
#include "commands/make_schema.hpp"
#include "commands/seeder.hpp"
#include "commands/make_resources.hpp"


int main(int argc, char *argv[]) {
    CLI::App app{"C++ Production Web Application CLI"};
    try {
        spdlog::info("CLI starting up...");

        std::string env_file = ".env";
        app.add_option("-e,--env", env_file, "Path to .env file")
            ->default_str(".env");

        app.add_subcommand("make",
                           "Scaffold project components and code generators");

        // Register all commands polymorphically
        std::vector<std::unique_ptr<ICommand>> commands;
        commands.push_back(std::make_unique<SeederCommand>());
        commands.push_back(std::make_unique<MakeSchemaCommand>());
        commands.push_back(std::make_unique<MakeResourcesCommand>());

        for (const auto &cmd : commands) {
            cmd->register_command(app);
        }

        app.parse(argc, argv);

        dotenv::init(env_file.c_str());
    } catch (const CLI::ParseError &e) {
        int code = app.exit(e);
        return (code == 3) ? 0 : code;
    } catch (const std::exception &e) {
        spdlog::error("Standard exception: {}", e.what());
        return 1;
    } catch (...) {
        spdlog::error("Unknown fatal error.");
        return 1;
    }

    return 0;
}

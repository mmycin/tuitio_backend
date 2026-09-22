#pragma once

#include "command.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <spdlog/spdlog.h>
#include <SQLiteCpp/SQLiteCpp.h>

class SeederCommand : public ICommand {
public:
    void register_command(CLI::App& app) override {
        auto* seed_cmd = app.add_subcommand("seed", "Run a database seeder script");
        
        // Bind option to member variable
        seed_cmd->add_option("-f,--file", seeder_file_, "Path to seeder SQL file")->required();

        // Bind the execution lifecycle to CLI11's callback
        seed_cmd->callback([this]() {
            this->execute();
        });
    }

private:
    std::string seeder_file_;

    void execute() {
        const char* env_db = std::getenv("DB_FILENAME");
        std::string db_filename = env_db ? env_db : "";

        if (db_filename.empty()) {
            spdlog::error("DB_FILENAME is not set in .env");
            std::exit(1);
        }

        std::ifstream file(seeder_file_);
        if (!file) {
            spdlog::error("Unable to open seeder file: {}", seeder_file_);
            std::exit(1);
        }

        std::stringstream buffer;
        buffer << file.rdbuf();

        try {
            SQLite::Database db(db_filename, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
            SQLite::Transaction transaction(db);

            db.exec(buffer.str());
            transaction.commit();

            spdlog::info("Seeder completed successfully.");
            spdlog::info("Database: {}", db_filename);
            spdlog::info("Seeder:   {}", seeder_file_);
        }
        catch (const SQLite::Exception& e) {
            spdlog::error("Seeder failed (SQLite error): {}", e.what());
            std::exit(1);
        }
        catch (const std::exception& e) {
            spdlog::error("Seeder failed (Standard error): {}", e.what());
            std::exit(1);
        }
    }
};
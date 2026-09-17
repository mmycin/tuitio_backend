#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <spdlog/spdlog.h>
#include <SQLiteCpp/SQLiteCpp.h>

inline std::string read_seeder_file(const std::string& filename) {
    std::ifstream file(filename);

    if (!file) {
        throw std::runtime_error("Unable to open seeder file: " + filename);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

inline int run_seeder(const std::string& seeder_file) {
    const char* env_db = std::getenv("DB_FILENAME");
    std::string db_filename = env_db ? env_db : "";

    if (db_filename.empty()) {
        spdlog::error("DB_FILENAME is not set in .env");
        return 1;
    }

    std::string sql;
    try {
        sql = read_seeder_file(seeder_file);
    }
    catch (const std::exception& e) {
        spdlog::error("{}", e.what());
        return 1;
    }

    try {
        SQLite::Database db(db_filename, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        SQLite::Transaction transaction(db);

        db.exec(sql);
        transaction.commit();

        spdlog::info("Seeder completed successfully.");
        spdlog::info("Database: {}", db_filename);
        spdlog::info("Seeder:   {}", seeder_file);
    }
    catch (const SQLite::Exception& e) {
        spdlog::error("Seeder failed (SQLite error): {}", e.what());
        return 1;
    }
    catch (const std::exception& e) {
        spdlog::error("Seeder failed (Standard error): {}", e.what());
        return 1;
    }

    return 0;
}
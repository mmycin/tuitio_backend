#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <spdlog/spdlog.h>
#include <SQLiteCpp/SQLiteCpp.h>
#include <dotenv.h>


struct Arguments {
    std::string file;
};

void print_usage(const char* program) {
    std::cout
        << "Usage: " << program
        << " --file <seeder.sql>\n\n"
        << "DB file is read from DB_FILENAME in .env\n\n"
        << "Example:\n"
        << "  " << program
        << " --file src/database/seeders/user_seeder.sql\n";
}

bool parse_arguments(int argc, char* argv[], Arguments& args) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--file") {
            if (i + 1 >= argc) {
                spdlog::error("--file requires a value");
                return false;
            }
            args.file = argv[++i];
        }
        else if (arg == "--help" || arg == "-h") {
            print_usage(argv[0]);
            std::exit(0);
        }
        else {
            spdlog::error("Unknown argument: {}", arg);
            return false;
        }
    }

    if (args.file.empty()) {
        spdlog::error("--file is required");
        return false;
    }

    return true;
}

std::string read_file(const std::string& filename) {
    std::ifstream file(filename);

    if (!file) {
        throw std::runtime_error("Unable to open seeder file: " + filename);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char* argv[]) {
    // 1. Initialize dotenv first so environment variables load into the process
    dotenv::init(".env");

    Arguments args;

    if (!parse_arguments(argc, argv, args)) {
        print_usage(argv[0]);
        return 1;
    }

    // 2. Read DB filename directly from the loaded environment variables
    const char* env_db = std::getenv("DB_FILENAME");
    std::string db_filename = env_db ? env_db : "";

    if (db_filename.empty()) {
        spdlog::error("DB_FILENAME is not set in .env");
        return 1;
    }

    // Read SQL file contents
    std::string sql;
    try {
        sql = read_file(args.file);
    }
    catch (const std::exception& e) {
        spdlog::error("{}", e.what());
        return 1;
    }

    try {
        // Open database safely using SQLiteCpp (handles resource release via RAII)
        SQLite::Database db(db_filename, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

        // Start transaction using SQLiteCpp's RAII Transaction wrapper
        SQLite::Transaction transaction(db);

        // Execute the entire SQL seed script
        db.exec(sql);

        // Commit transaction
        transaction.commit();

        spdlog::info("Seeder completed successfully.");
        spdlog::info("Database: {}", db_filename);
        spdlog::info("Seeder:   {}", args.file);
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
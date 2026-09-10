#include <sqlite3.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "utils/env.hpp"

struct Arguments {
    std::string file;
};

void print_usage(const char* program) {
    std::cerr
        << "Usage: " << program
        << " --file <seeder.sql>\n\n"
        << "DB file is read from DB_FILENAME in .env\n\n"
        << "Example:\n"
        << "  " << program
        << " --file src/database/seeds/dev.sql\n";
}

bool parse_arguments(int argc, char* argv[], Arguments& args) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--file") {
            if (i + 1 >= argc) {
                std::cerr << "Error: --file requires a value\n";
                return false;
            }

            args.file = argv[++i];
        }
        else if (arg == "--help" || arg == "-h") {
            print_usage(argv[0]);
            std::exit(0);
        }
        else {
            std::cerr << "Error: unknown argument: " << arg << "\n";
            return false;
        }
    }

    if (args.file.empty()) {
        std::cerr << "Error: --file is required\n";
        return false;
    }

    return true;
}

std::string read_file(const std::string& filename) {
    std::ifstream file(filename);

    if (!file) {
        throw std::runtime_error(
            "Unable to open seeder file: " + filename
        );
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

int main(int argc, char* argv[]) {
    Arguments args;

    if (!parse_arguments(argc, argv, args)) {
        print_usage(argv[0]);
        return 1;
    }

    // Read DB filename from .env
    std::string db_filename = getEnv("DB_FILENAME");

    if (db_filename.empty()) {
        std::cerr << "Error: DB_FILENAME is not set in .env\n";
        return 1;
    }

    // Read SQL file.
    std::string sql;

    try {
        sql = read_file(args.file);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    // Open database.
    sqlite3* db = nullptr;

    int rc = sqlite3_open(db_filename.c_str(), &db);

    if (rc != SQLITE_OK) {
        std::cerr
            << "Error: unable to open database '"
            << db_filename
            << "': "
            << sqlite3_errmsg(db)
            << "\n";

        sqlite3_close(db);
        return 1;
    }

    // Start transaction.
    char* error_message = nullptr;

    rc = sqlite3_exec(
        db,
        "BEGIN TRANSACTION;",
        nullptr,
        nullptr,
        &error_message
    );

    if (rc != SQLITE_OK) {
        std::cerr
            << "Error starting transaction: "
            << error_message
            << "\n";

        sqlite3_free(error_message);
        sqlite3_close(db);
        return 1;
    }

    // Execute seed SQL.
    rc = sqlite3_exec(
        db,
        sql.c_str(),
        nullptr,
        nullptr,
        &error_message
    );

    if (rc != SQLITE_OK) {
        std::cerr
            << "Seeder failed: "
            << error_message
            << "\n";

        sqlite3_free(error_message);

        sqlite3_exec(
            db,
            "ROLLBACK;",
            nullptr,
            nullptr,
            nullptr
        );

        sqlite3_close(db);
        return 1;
    }

    // Commit.
    rc = sqlite3_exec(
        db,
        "COMMIT;",
        nullptr,
        nullptr,
        &error_message
    );

    if (rc != SQLITE_OK) {
        std::cerr
            << "Error committing transaction: "
            << error_message
            << "\n";

        sqlite3_free(error_message);
        sqlite3_exec(
            db,
            "ROLLBACK;",
            nullptr,
            nullptr,
            nullptr
        );

        sqlite3_close(db);
        return 1;
    }

    sqlite3_close(db);

    std::cout
        << "Seeder completed successfully.\n"
        << "Database: " << db_filename << "\n"
        << "Seeder:   " << args.file << "\n";

    return 0;
}

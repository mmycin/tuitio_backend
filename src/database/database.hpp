#pragma once

#include <memory>

#include <SQLiteCpp/SQLiteCpp.h>

#include "utils/env.hpp"

inline SQLite::Database& getDB()
{
    static std::unique_ptr<SQLite::Database> db;

    if (!db) {
        db = std::make_unique<SQLite::Database>(
            getEnv("DB_FILENAME"),
            SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE
        );
    }

    return *db;
}

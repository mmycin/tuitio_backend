#pragma once

#include <memory>

#include <SQLiteCpp/SQLiteCpp.h>
#include "configs/db_config.hpp"

inline SQLite::Database& getDB()
{
    static std::unique_ptr<SQLite::Database> db;

    if (!db) {
        db = std::make_unique<SQLite::Database>(
            DBConfig::DB_FILENAME
        );
    }

    return *db;
}

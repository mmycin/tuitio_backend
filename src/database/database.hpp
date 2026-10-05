#pragma once

#include <memory>
#include <string>
#include <SQLiteCpp/SQLiteCpp.h>
#include "SQLiteCpp/Database.h"
#include "configs/db_config.hpp"

inline SQLite::Database& getDB()
{
    thread_local std::unique_ptr<SQLite::Database> db;

    if (!db) {
        std::string filename = DBConfig::DB_FILENAME();
        if (filename.empty()) {
            filename = "app.db";
        }
        db = std::make_unique<SQLite::Database>(
            filename,
            SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE
        );
        db->setBusyTimeout(5000);
        db->exec("PRAGMA journal_mode=WAL;");
        db->exec("PRAGMA foreign_keys=ON;");
    }

    return *db;
}


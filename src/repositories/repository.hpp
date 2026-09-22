#pragma once

#include <SQLiteCpp/Database.h>
#include "database/database.hpp"

class IRepository {
protected:
    SQLite::Database &db = getDB();

public:
    virtual ~IRepository() = default;
};
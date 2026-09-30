#pragma once

#include "database/database.hpp"
#include <SQLiteCpp/Database.h>
#include <SQLiteCpp/SQLiteCpp.h>

class IRepository {
  protected:
    SQLite::Database &db = getDB();

  public:
    virtual ~IRepository() = default;
};

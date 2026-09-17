#pragma once

#include "SQLiteCpp/Database.h"
#include "database/database.hpp"
#include "models/user_model.hpp"
#include <SQLiteCpp/Statement.h>

class UserRepository {
  private:
    SQLite::Database &db = getDB();

  public:
    User getUserById(int id) {
        User user(id);
        SQLite::Statement query(this->db, R"SQL(
        	SELECT name, email
         	FROM users
          	WHERE id = ?;
		)SQL");
        query.bind(1, id);

        try {
            while (query.executeStep()) {
                user.name = query.getColumn(0).getString();
                user.email = query.getColumn(1).getString();
            }
        } catch (std::exception &e) {
            cout << "exception: " << e.what() << endl;
        }
        return user;
    }
};

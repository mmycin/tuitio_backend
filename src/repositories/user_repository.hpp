#pragma once

#include "models/user_model.hpp"
#include "repositories/repository.hpp"
#include "spdlog/spdlog.h"
#include <SQLiteCpp/Statement.h>

using namespace std;

class UserRepository : public IRepository {
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
            spdlog::error("exception: {}", e.what());
        }
        return user;
    }

    User createUser(User user) {
        SQLite::Statement query(this->db, R"SQL(
        	INSERT INTO users
        	(name, email, password_hash)
       		VALUES
         	(?, ?, ?)
         	RETURNING *;
        )SQL");
        query.bind(1, user.name);
        query.bind(2, user.email);
        query.bind(3, user.password_hash);

        try {
            while (query.executeStep()) {
                user.id = query.getColumn(0);
            }
        } catch (std::exception &e) {
            spdlog::error("exception: {}", e.what());
        }

        if (user.id == 0) {
            spdlog::error("User creation failed");
        }

        return user;
    }

    User getUserByEmail(string email) {
        User user(0);

        SQLite::Statement query(this->db, R"SQL(
        	SELECT id, name, email, password_hash
         	FROM users
          	WHERE email = ?;
        )SQL");
        query.bind(1, email);

        try {
            while (query.executeStep()) {
                user.id = query.getColumn(0);
                user.name = query.getColumn(1).getString();
                user.email = query.getColumn(2).getString();
                user.password_hash = query.getColumn(3).getString();
            }
        } catch (std::exception &e) {
            spdlog::error("exception: {}", e.what());
        }

        return user;
    }
};

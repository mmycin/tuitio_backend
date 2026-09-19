#pragma once

#include "errors/api.hpp"
#include "models/user_model.hpp"
#include "repositories/repository.hpp"
#include "spdlog/spdlog.h"
#include <SQLiteCpp/Statement.h>

using namespace std;

class UserRepository : public IRepository {
  public:
    User getUserById(int id) {
        User user(id);
        try {
            SQLite::Statement query(this->db, R"SQL(
                SELECT name, email
                FROM users
                WHERE id = ?;
            )SQL");
            query.bind(1, id);

            while (query.executeStep()) {
                user.name = query.getColumn(0).getString();
                user.email = query.getColumn(1).getString();
            }
        } catch (const std::exception &e) {
            spdlog::error("Database error in getUserById: {}", e.what());
            throw ApiError("Database error while retrieving user", 500);
        }
        return user;
    }

    User createUser(User user) {
        try {
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

            while (query.executeStep()) {
                user.id = query.getColumn(0);
            }
        } catch (const std::exception &e) {
            spdlog::error("Database error in createUser: {}", e.what());
            throw ApiError("Database error while creating user", 500);
        }

        return user;
    }

    User getUserByEmail(string email) {
        User user(0);
        try {
            SQLite::Statement query(this->db, R"SQL(
            	SELECT id, name, email, password_hash
             	FROM users
              	WHERE email = ?;
            )SQL");
            query.bind(1, email);

            while (query.executeStep()) {
                user.id = query.getColumn(0);
                user.name = query.getColumn(1).getString();
                user.email = query.getColumn(2).getString();
                user.password_hash = query.getColumn(3).getString();
            }
        } catch (const std::exception &e) {
            spdlog::error("Database error in getUserByEmail: {}", e.what());
            throw ApiError("Database error while looking up user by email",
                           500);
        }

        return user;
    }

    bool deleteUser(int id) {
        User user;
        try {
            SQLite::Statement query(this->db, R"SQL(
	        	DELETE FROM users
				WHERE id = ?
				RETURNING *;
            )SQL");
            query.bind(1, id);

            while(query.executeStep()) {
            	user.id = query.getColumn(0);
            }

            if(user.id == 0) {
            	return false;
            } else {
            	return true;
            }
            
        } catch (const std::exception &e) {
            spdlog::error("Database error in getUserByEmail: {}", e.what());
            throw ApiError("Database error while looking up user by email",
                           500);
        }
    }
};

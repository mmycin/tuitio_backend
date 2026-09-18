#pragma once

#include "models/user_model.hpp"
#include "repositories/repository.hpp"
#include "spdlog/spdlog.h"
#include <SQLiteCpp/Statement.h>
#include <iostream>


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
            cout << "exception: " << e.what() << endl;
        }
        return user;
    }

    void createUser(User user) {
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
            cout << "exception: " << e.what() << endl;
        }

        if(user.id == 0) {
        	spdlog::error("User creation failed");
        }
    }
};

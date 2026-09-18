#pragma once

#define _HAS_STD_BYTE 0

#include "repositories/user_repository.hpp"
#include "services/service.hpp"
#include "utils/bearer_token.hpp"
#include "utils/password_hash.hpp"
#include <httplib.h>
#include <memory>
#include <tuple>


class AuthService : public IService {
  public:
    explicit AuthService(std::unique_ptr<UserRepository> repo,
                         std::unique_ptr<AuthManager> auth_manager)
        : repo(std::move(repo)), auth_manager(std::move(auth_manager)) {}

    std::tuple<User, string> loginUser(string email, string password) {
        User user = this->repo->getUserByEmail(email);
        if (user.id == 0) {
            return { User(0), "User Not found" };
        }

        if (PasswordHash::verify(password, user.password_hash)) {
            string token = this->auth_manager->make_token(user.id);
            return { user, token };
        } else {
            return { User(0), "Invalid Credentials" };
        }

        return { User(0), "Something went wrong" };
    }

    User verifyUser(const httplib::Request &req) {
        string auth_header = req.get_header_value("Authorization");
        string token = "";
        User user;

        // Check if the header starts with "Bearer " and has content after it
        if (auth_header.rfind("Bearer ", 0) == 0 && auth_header.size() > 7) {
            token = auth_header.substr(7);
        } else {
            spdlog::warn("Invalid or missing Authorization header format.");
        }

        auto id = this->auth_manager->get_id(token);
        if(!id) {
        	return user;
        }

        user = this->repo->getUserById(*id);
        return user;
    }

  private:
    std::unique_ptr<UserRepository> repo;
    std::unique_ptr<AuthManager> auth_manager;
};

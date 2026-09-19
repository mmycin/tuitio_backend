#pragma once

#define _HAS_STD_BYTE 0

#include "errors/api.hpp"
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
            throw NotFoundError("User not found with the provided email");
        }

        if (PasswordHash::verify(password, user.password_hash)) {
            string token = this->auth_manager->make_token(user.id);
            return { user, token };
        } else {
            throw UnauthorizedError("Invalid credentials");
        }
    }

    User verifyUser(const httplib::Request &req) {
        string auth_header = req.get_header_value("Authorization");
        string token = "";

        if (auth_header.rfind("Bearer ", 0) == 0 && auth_header.size() > 7) {
            token = auth_header.substr(7);
        } else {
            throw UnauthorizedError("Invalid or missing Authorization header format");
        }

        auto id = this->auth_manager->get_id(token);
        if (!id) {
            throw UnauthorizedError("Invalid or expired token");
        }

        User user = this->repo->getUserById(*id);
        if (user.id == 0) {
            throw NotFoundError("User not found");
        }
        return user;
    }

    User signUp(string name, string email, string password) {
        User existing = this->repo->getUserByEmail(email);
        if (existing.id != 0) {
            nlohmann::json details;
            details["email"] = "Email is already registered";
            throw ValidationError(details, "Validation failed");
        }

        auto password_hash = PasswordHash::hash(password);
        auto user = this->repo->createUser(User(name, email, password_hash));

        if (user.id == 0) {
            throw ApiError("Failed to create user account", 500);
        }

        return user;
    }

  private:
    std::unique_ptr<UserRepository> repo;
    std::unique_ptr<AuthManager> auth_manager;
};

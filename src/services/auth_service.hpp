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

    // verifyUser: token already validated by auth middleware.
    // Read the pre-injected X-User-Id header instead of re-decrypting.
    User verifyUser(const httplib::Request &req) {
        std::string user_id_hdr = req.get_header_value("X-User-Id");
        if (user_id_hdr.empty()) {
            throw UnauthorizedError("Invalid or expired token");
        }

        uint64_t id = 0;
        try {
            id = std::stoull(user_id_hdr);
        } catch (...) {
            throw UnauthorizedError("Invalid authentication context");
        }

        User user = this->repo->getUserById(static_cast<int>(id));
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

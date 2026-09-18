#pragma once

#include "repositories/user_repository.hpp"
#include "services/service.hpp"
#include "utils/bearer_token.hpp"
#include "utils/password_hash.hpp"
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

  private:
    std::unique_ptr<UserRepository> repo;
    std::unique_ptr<AuthManager> auth_manager;
};

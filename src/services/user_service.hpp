#pragma once

#include "dtos/user_dto.hpp"
#include "errors/api.hpp"
#include "models/user_model.hpp"
#include "password_hash.hpp"
#include "repositories/user_repository.hpp"
#include "services/service.hpp"
#include "utils/bearer_token.hpp"
#include <memory>

class UserService : public IService {
  public:
    explicit UserService(std::unique_ptr<UserRepository> repo,
                         std::unique_ptr<AuthManager> auth_manager)
        : repo(std::move(repo)), auth_manager(std::move(auth_manager)) {}

    User getUserById(int id) {
        User user = this->repo->getUserById(id);
        if (user.id == 0) {
            throw NotFoundError("User not found with id: " +
                                std::to_string(id));
        }
        return user;
    }

    bool deleteUserById(string token, int id) {
        auto userId_opt = this->auth_manager->get_id(token);
        if (!userId_opt) {
            throw UnauthorizedError(
                "You are not authenticated to delete the user");
        }
        int userId = *userId_opt;

        if (userId != id) {
            throw ValidationError(
                "Your requested user can not be deleted by you");
        }

        return this->repo->deleteUser(id);
    }

    User updateUser(string token, int id, UpdateUserRequest &req) {
        auto userId_opt = this->auth_manager->get_id(token);
        if (!userId_opt) {
            throw UnauthorizedError(
                "You are not authenticated to delete the user");
        }
        int userId = *userId_opt;

        if (userId != id) {
            throw ValidationError(
                "Your requested user can not be deleted by you");
        }

        if (req.password != "") {
            req.password = PasswordHash::hash(req.password);
        }

        return this->repo->updateUser(id, req);
    }

  private:
    std::unique_ptr<UserRepository> repo;
    std::unique_ptr<AuthManager> auth_manager;
};

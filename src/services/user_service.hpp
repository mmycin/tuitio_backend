#pragma once

#include "dtos/user_dto.hpp"
#include "errors/api.hpp"
#include "models/user_model.hpp"
#include "password_hash.hpp"
#include "repositories/user_repository.hpp"
#include "services/service.hpp"
#include <cstdint>
#include <memory>

class UserService : public IService {
  public:
    explicit UserService(std::unique_ptr<UserRepository> repo)
        : repo(std::move(repo)) {}

    User getUserById(int id) {
        User user = this->repo->getUserById(id);
        if (user.id == 0) {
            throw NotFoundError("User not found with id: " + std::to_string(id));
        }
        return user;
    }

    // authId is the user id extracted by the auth middleware (already validated).
    bool deleteUserById(uint64_t authId, int id) {
        if (static_cast<int>(authId) != id) {
            throw ValidationError("Your requested user can not be deleted by you");
        }
        return this->repo->deleteUser(id);
    }

    // authId is the user id extracted by the auth middleware (already validated).
    User updateUser(uint64_t authId, int id, UpdateUserRequest &req) {
        if (static_cast<int>(authId) != id) {
            throw ValidationError("Your requested user can not be updated by you");
        }

        if (req.password != "") {
            req.password = PasswordHash::hash(req.password);
        }

        return this->repo->updateUser(id, req);
    }

  private:
    std::unique_ptr<UserRepository> repo;
};

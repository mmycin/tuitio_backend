#pragma once

#include "errors/api.hpp"
#include "models/user_model.hpp"
#include "repositories/user_repository.hpp"
#include "services/service.hpp"
#include <memory>


class UserService : public IService {
  public:
    explicit UserService(std::unique_ptr<UserRepository> repo)
        : repo(std::move(repo)) {}

    User getUserById(int id) {
        User user = this->repo->getUserById(id);
        if (user.id == 0) {
            throw NotFoundError("User not found with id: " +
                                std::to_string(id));
        }
        return user;
    }

    bool deleteUserById(int id) {
        bool isDeleted = this->repo->deleteUser(id);
        return isDeleted;
    }

  private:
    std::unique_ptr<UserRepository> repo;
};

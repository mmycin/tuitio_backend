#pragma once

#include "errors/api.hpp"
#include <memory>
#include "repositories/user_repository.hpp"
#include "models/user_model.hpp"
#include "services/service.hpp"

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

private:
    std::unique_ptr<UserRepository> repo;
};
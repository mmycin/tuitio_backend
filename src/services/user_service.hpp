#pragma once

#include <memory>
#include "repositories/user_repository.hpp"
#include "models/user_model.hpp"
#include "services/service.hpp"

class UserService : public IService {
public:
    explicit UserService(std::unique_ptr<UserRepository> repo) 
        : repo(std::move(repo)) {}

    User getUserById(int id) {
        return this->repo->getUserById(id);
    }

private:
    std::unique_ptr<UserRepository> repo;
};
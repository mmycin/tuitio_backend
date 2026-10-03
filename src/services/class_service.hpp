#pragma once

#include "errors/api.hpp"
#include <memory>
#include "repositories/class_repository.hpp"
#include "services/service.hpp"

class ClassService : public IService {
public:
    explicit ClassService(std::unique_ptr<ClassRepository> repo)
        : repo(std::move(repo)) {}


private:
    std::unique_ptr<ClassRepository> repo;
};

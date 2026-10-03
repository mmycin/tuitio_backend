#pragma once

#include "errors/api.hpp"
#include "repositories/class_repository.hpp"
#include "services/service.hpp"
#include <memory>

class ClassService : public IService {
  public:
    explicit ClassService(std::unique_ptr<ClassRepository> repo)
        : repo(std::move(repo)) {}

    Class createClass(Class inputClass) {
        return this->repo->createClass(inputClass);
    }

    std::vector<Class> getClassesByCycleID(int id) {
        return this->repo->getClassesByCycleID(id);
    }

    Class getClassByID(int id) {
        return this->repo->getClassByID(id);
    }

    bool deleteClass(int id) {
        return this->repo->deleteClass(id);
    }

  private:
    std::unique_ptr<ClassRepository> repo;
};

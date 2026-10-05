#pragma once

#include "errors/api.hpp"
#include "repositories/class_repository.hpp"
#include "repositories/cycle_repository.hpp"
#include "repositories/student_repository.hpp"
#include "services/service.hpp"
#include "utils/bearer_token.hpp"
#include <memory>

class ClassService : public IService {
  public:
    explicit ClassService(std::unique_ptr<ClassRepository> repo,
                          std::unique_ptr<AuthManager> auth_manager)
        : repo(std::move(repo)), auth_manager(std::move(auth_manager)) {}

    void verifyCycleOwnership(const std::string &token, int cycle_id) {
        auto user_id = auth_manager->get_id(token);
        if (!user_id) {
            throw UnauthorizedError("Invalid or expired token");
        }
        CycleRepository cycle_repo;
        Cycle cycle = cycle_repo.getCycleByID(cycle_id);
        if (cycle.id == 0) {
            throw NotFoundError("Cycle not found");
        }
        StudentRepository student_repo;
        auto student = student_repo.getStudentById(*user_id, cycle.student_id);
        if (student.id == 0) {
            throw UnauthorizedError("You are not authorized to access classes for this cycle");
        }
    }

    Class createClass(const std::string &token, Class inputClass) {
        verifyCycleOwnership(token, inputClass.cycle_id);
        return this->repo->createClass(inputClass);
    }

    std::vector<Class> getClassesByCycleID(const std::string &token, int id) {
        verifyCycleOwnership(token, id);
        return this->repo->getClassesByCycleID(id);
    }

    Class getClassByID(const std::string &token, int id) {
        Class cls = this->repo->getClassByID(id);
        if (cls.id != 0) {
            verifyCycleOwnership(token, cls.cycle_id);
        }
        return cls;
    }

    bool deleteClass(const std::string &token, int id) {
        Class cls = this->repo->getClassByID(id);
        if (cls.id != 0) {
            verifyCycleOwnership(token, cls.cycle_id);
        }
        return this->repo->deleteClass(id);
    }

  private:
    std::unique_ptr<ClassRepository> repo;
    std::unique_ptr<AuthManager> auth_manager;
};


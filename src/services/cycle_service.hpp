#pragma once

#include "errors/api.hpp"
#include "models/cycles_model.hpp"
#include "repositories/cycle_repository.hpp"
#include "repositories/student_repository.hpp"
#include "services/service.hpp"
#include "utils/bearer_token.hpp"

#include <memory>
#include <vector>

class CycleService : public IService {
  public:
    explicit CycleService(std::unique_ptr<CycleRepository> repo,
                          std::unique_ptr<AuthManager> auth_manager)
        : repo(std::move(repo)), auth_manager(std::move(auth_manager)) {}

    std::vector<Cycle> getCyclesByIDs(const std::string &token, std::vector<int> ids, int student_id) {
        auto user_id = this->auth_manager->get_id(token);
        if (!user_id) {
            throw UnauthorizedError("Invalid or expired token");
        }
        StudentRepository student_repo;
        auto student = student_repo.getStudentById(*user_id, student_id);
        if (student.id == 0) {
            throw UnauthorizedError("You are not authorized to access cycles for this student");
        }
        return this->repo->getCyclesByIDs(ids, student_id);
    }

    Cycle updateCycle(const std::string &token, int id, bool is_paid) {
        auto user_id = this->auth_manager->get_id(token);
        if (!user_id) {
            throw UnauthorizedError("Invalid or expired token");
        }
        Cycle existing = this->repo->getCycleByID(id);
        if (existing.id == 0) {
            throw NotFoundError("Cycle not found");
        }
        StudentRepository student_repo;
        auto student = student_repo.getStudentById(*user_id, existing.student_id);
        if (student.id == 0) {
            throw UnauthorizedError("You are not authorized to update this cycle");
        }
        return this->repo->updateCycle(id, is_paid);
    }

  private:
    std::unique_ptr<CycleRepository> repo;
    std::unique_ptr<AuthManager> auth_manager;
};


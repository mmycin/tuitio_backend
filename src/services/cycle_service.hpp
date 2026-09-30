#pragma once

#include "errors/api.hpp"
#include "models/cycles_model.hpp"
#include "repositories/cycle_repository.hpp"
#include "services/service.hpp"
#include "utils/bearer_token.hpp"

#include <memory>
#include <vector>

class CycleService : public IService {
  public:
    explicit CycleService(std::unique_ptr<CycleRepository> repo,
                          std::unique_ptr<AuthManager> auth_manager)
        : repo(std::move(repo)), auth_manager(std::move(auth_manager)) {}

    std::vector<Cycle> getCyclesByIDs(std::vector<int> ids, int student_id) {
        return this->repo->getCyclesByIDs(ids, student_id);
    }

    Cycle updateCycle(int id, bool is_paid) {
        return this->repo->updateCycle(id, is_paid);
    }

  private:
    std::unique_ptr<CycleRepository> repo;
    std::unique_ptr<AuthManager> auth_manager;
};

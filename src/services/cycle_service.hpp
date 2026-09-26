#pragma once

#include "errors/api.hpp"
#include <memory>
#include "repositories/cycle_repository.hpp"
#include "services/service.hpp"
#include "models/cycles_model.hpp"
#include <vector>

class CycleService : public IService {
public:
    explicit CycleService(std::unique_ptr<CycleRepository> repo)
        : repo(std::move(repo)) {}

    std::vector<Cycle> getCyclesByIDs(std::vector<int> ids, int student_id) {
        return this->repo->getCyclesByIDs(ids, student_id);
    }

private:
    std::unique_ptr<CycleRepository> repo;
};

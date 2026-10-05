#pragma once

#include "SQLiteCpp/Transaction.h"
#include "errors/api.hpp"
#include "models/classes_model.hpp"
#include "models/cycles_model.hpp"
#include "repositories/class_repository.hpp"
#include "repositories/repository.hpp"
#include "utils/time_utls.hpp"
#include <SQLiteCpp/Statement.h>
#include <chrono>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

class CycleRepository : public IRepository {
  public:
    std::vector<Cycle> getCyclesByIDs(std::vector<int> ids, int student_id) {
        std::vector<Cycle> cycles;
        string sql = "";

        if (!ids.empty()) {
            sql = R"SQL(
                    SELECT * FROM cycles
                    WHERE student_id = ? AND
                    id IN (
                )SQL";

            for (size_t i = 0; i < ids.size(); ++i) {
                if (i > 0) {
                    sql += ", ";
                }
                sql += "?";
            }

            sql += R"SQL(
                );
            )SQL";
        } else {
            sql = R"SQL(
                SELECT * FROM cycles
                WHERE student_id = ?;
            )SQL";
        }

        SQLite::Statement query(this->db, sql);

        if (!ids.empty()) {
            query.bind(1, student_id);
            for (size_t i = 0; i < ids.size(); ++i) {
                query.bind(static_cast<int>(i + 2), ids[i]);
            }
        } else {
            query.bind(1, student_id);
        }

        while (query.executeStep()) {
            int id = query.getColumn(0);
            int student_id = query.getColumn(1);
            std::optional<std::chrono::system_clock::time_point> started_at_opt;
            if (!query.getColumn(2).isNull()) {
                started_at_opt = TimeConverter::sqliteToTimePoint(
                    query.getColumn(2).getString());
            }
            auto started_at = *started_at_opt;
            int class_count = query.getColumn(3);
            bool is_paid = query.getColumn(4).getInt() != 0;

            Cycle cycle;
            cycle.id = id;
            cycle.student_id = student_id;
            cycle.started_at = started_at;
            cycle.class_count = class_count;
            cycle.is_paid = is_paid;

            cycles.push_back(cycle);
        }

        return cycles;
    }

    Cycle getCycleByID(int id) {
        Cycle cycle;

        SQLite::Statement query(this->db, R"SQL(
            SELECT * FROM cycles
            WHERE id = ?;
        )SQL");
        query.bind(1, id);

        while (query.executeStep()) {
            int id = query.getColumn(0);
            int student_id = query.getColumn(1);
            std::optional<std::chrono::system_clock::time_point> started_at_opt;
            if (!query.getColumn(2).isNull()) {
                started_at_opt = TimeConverter::sqliteToTimePoint(
                    query.getColumn(2).getString());
            }
            auto started_at = *started_at_opt;
            int class_count = query.getColumn(3);
            bool is_paid = query.getColumn(4).getInt() != 0;

            cycle.id = id;
            cycle.student_id = student_id;
            cycle.started_at = started_at;
            cycle.class_count = class_count;
            cycle.is_paid = is_paid;
        }

        return cycle;
    }

    Cycle createCycle(Cycle cycle) {
        SQLite::Statement query(this->db, R"SQL(
            INSERT INTO cycles
            (student_id, started_at, class_count, is_paid)
            VALUES
            (?, ?, ?, false)
            RETURNING *;
        )SQL");
        query.bind(1, cycle.student_id);
        query.bind(2, TimeConverter::timePointToString(cycle.started_at));
        query.bind(3, cycle.class_count);

        while (query.executeStep()) {
            cycle.id = query.getColumn(0);
        }

        return cycle;
    }

    int getClassCountFromCycleID(int id) {
        int class_count;

        SQLite::Statement query(this->db, R"SQL(
            SELECT COUNT(*) AS class_count
            FROM classes
            WHERE cycle_id = ?;
        )SQL");
        query.bind(1, id);

        while (query.executeStep()) {
            class_count = query.getColumn(0).getInt();
        }

        return class_count;
    }

  
    std::tuple<Cycle, Cycle> splitCycle(Cycle oldCycle) {
        SQLite::Transaction tx(this->db);
        int actual_count = getClassCountFromCycleID(oldCycle.id);
        int class_count = oldCycle.class_count;

        if (actual_count <= class_count) {
            tx.commit();
            return { oldCycle, Cycle() };
        }

        Cycle newCycle;
        newCycle.student_id = oldCycle.student_id;
        newCycle.class_count = oldCycle.class_count;
        newCycle.is_paid = false;
        newCycle.started_at = chrono::system_clock::now();

        auto newCycleCreated = this->createCycle(newCycle);
        if(newCycleCreated.id == 0) {
            throw ApiError("Can not properly create new cycle", 501);
        }

        int extraClasseCount = actual_count - class_count;

        ClassRepository classRepo;
        std::vector<Class> remainingClasses = classRepo.getRemainingClasses(oldCycle.id, extraClasseCount);

        for(auto& remainingClass: remainingClasses) {
            auto is_updated = classRepo.updateCycleID(remainingClass, newCycleCreated.id);
            if(is_updated == false) {
                throw ApiError("Can not create new cycle for remaining classes", 501);
            }
        }
        
        tx.commit();
        return { oldCycle, newCycleCreated };
    }

    Cycle updateCycle(int id, bool is_paid) {
        Cycle cycle = getCycleByID(id);

        if (cycle.id == 0) {
            throw NotFoundError("Cycle not found by id");
        }

        SQLite::Statement query(this->db, R"SQL(
            UPDATE cycles
            SET is_paid = ?
            WHERE id = ?
            RETURNING is_paid;
        )SQL");
        query.bind(1, is_paid);
        query.bind(2, id);

        while (query.executeStep()) {
            cycle.is_paid = query.getColumn(0).getInt() != 0;
        }

        if (is_paid == true) {
            auto [oldCycle, newCycle] = splitCycle(cycle);
            if(newCycle.id == 0) {
                spdlog::info("No new cycle created");
            } else {
                cycle = newCycle;
                spdlog::info("New cycle created");
            }
        }

        return cycle;
    }
};

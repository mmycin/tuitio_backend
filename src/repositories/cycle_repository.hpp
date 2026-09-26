#pragma once

#include "errors/api.hpp"
#include "models/cycles_model.hpp"
#include "repositories/repository.hpp"
#include "spdlog/spdlog.h"
#include <SQLiteCpp/Statement.h>
#include <chrono>
#include <optional>
#include <string>
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
            for (size_t i = 1; i < ids.size(); ++i) {
                query.bind(static_cast<int>(i + 1), ids[i]);
            }
        } else {
            query.bind(1, student_id);
        }

        while (query.executeStep()) {
            int id = query.getColumn(0);
            int student_id = query.getColumn(1);
            std::optional<std::chrono::system_clock::time_point> started_at_opt;
            if (!query.getColumn(2).isNull()) {
                started_at_opt =
                    sqliteToTimePoint(query.getColumn(2).getString());
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
};

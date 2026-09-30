#pragma once

#include "models/cycles_model.hpp"
#include "repositories/repository.hpp"
#include "utils/time_utls.hpp"
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
                started_at_opt =
                    TimeConverter::sqliteToTimePoint(query.getColumn(2).getString());
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


        while(query.executeStep()) {
            cycle.id = query.getColumn(0);
        }
        
        return cycle;
    }
};

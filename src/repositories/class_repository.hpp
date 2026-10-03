#pragma once

#include "errors/api.hpp"
#include "models/classes_model.hpp"
#include "repositories/repository.hpp"
#include "spdlog/spdlog.h"
#include "utils/time_utls.hpp"
#include <SQLiteCpp/Statement.h>
#include <vector>

using namespace std;

class ClassRepository : public IRepository {
  public:
    Class createClass(Class inputClass) {
        SQLite::Statement query(this->db, R"SQL(
                INSERT INTO classes
                (cycle_id, created_at, notes)
                VALUES
                (?, ?, ?)
                RETURNING *;
            )SQL");
        query.bind(1, inputClass.cycle_id);
        query.bind(2, TimeConverter::timePointToString(inputClass.created_at));
        query.bind(3, inputClass.notes);

        while (query.executeStep()) {
            inputClass.id = query.getColumn(0);
        }

        return inputClass;
    }

    std::vector<Class> getClassesByCycleID(int id) {
        std::vector<Class> classes;

        SQLite::Statement query(this->db, R"SQL(
                SELECT * FROM classes
                WHERE cycle_id = ?;
            )SQL");
        query.bind(1, id);

        while (query.executeStep()) {
            Class singleClass;

            singleClass.id = query.getColumn(0);
            singleClass.cycle_id = query.getColumn(1);
            singleClass.created_at = TimeConverter::sqliteToTimePoint(
                query.getColumn(2).getString());
            singleClass.notes = query.getColumn(3).getString();

            classes.push_back(singleClass);
        }

        return classes;
    }

    std::vector<Class> getRemainingClasses(int cycle_id, int limit) {
        std::vector<Class> remainingClasses;

        SQLite::Statement query(this->db, R"SQL(
            SELECT * FROM classes
            WHERE cycle_id = ?
            ORDER BY id DESC
            LIMIT ?;
        )SQL");
        query.bind(1, cycle_id);
        query.bind(2, limit);

        while (query.executeStep()) {
            Class singleClass;

            singleClass.id = query.getColumn(0);
            singleClass.cycle_id = query.getColumn(1);
            singleClass.created_at = TimeConverter::sqliteToTimePoint(
                query.getColumn(2).getString());
            singleClass.notes = query.getColumn(3).getString();

            remainingClasses.push_back(singleClass);
            
        }
        
        return remainingClasses;
    } 


    Class getClassByID(int id) {
        Class outputClass;

        SQLite::Statement query(this->db, R"SQL(
                SELECT * FROM classes
                WHERE id = ?;
            )SQL");
        query.bind(1, id);

        while (query.executeStep()) {
            outputClass.id = query.getColumn(0);
            outputClass.cycle_id = query.getColumn(1);
            outputClass.created_at = TimeConverter::sqliteToTimePoint(
                query.getColumn(2).getString());
            outputClass.notes = query.getColumn(3).getString();
        }

        return outputClass;
    }

    bool deleteClass(int id) {
        Class outputClass;

        SQLite::Statement query(this->db, R"SQL(
                DELETE FROM classes
                WHERE id = ?
                RETURNING *;
            )SQL");
        query.bind(1, id);

        while (query.executeStep()) {
            outputClass.id = query.getColumn(0);
        }

        return outputClass.id == 0 ? false : true;
    }

    bool updateCycleID(Class inputClass, int newCycleID) {
        Class outputClass;

        SQLite::Statement query(this->db, R"SQL(
            UPDATE classes  
            SET cycle_id = ?
            WHERE id = ?
            RETURNING *;
        )SQL");
        query.bind(1, newCycleID);
        query.bind(2, inputClass.id);

        while (query.executeStep()) {
            outputClass.id = query.getColumn(0);
        }

        return outputClass.id == 0 ? false : true;
    }
};

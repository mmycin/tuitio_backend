#pragma once

#include "errors/api.hpp"
#include "spdlog/spdlog.h"
#include <SQLiteCpp/Statement.h>
#include "repositories/repository.hpp"
#include "models/students_model.hpp"
#include <vector>

using namespace std;

class StudentRepository : public IRepository {
    public:
        std::vector<Student> getStudents(int id) {
            std::vector<Student> students;

            SQLite::Statement query(this->db, R"SQL(
                SELECT * FROM students
                WHERE user_id = ?;
            )SQL");
            query.bind(1, id);

            while(query.executeStep()) {
                int id = query.getColumn(0);
                string name = query.getColumn(2).getString();
                int fee = query.getColumn(3);

                Student student(id);
                student.name = name;
                student.fee = fee;

                students.push_back(student);
            }

            return students;
        }
};
